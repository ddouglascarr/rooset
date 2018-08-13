package eventproc

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
)

// EventProcessor transforms a slice of event containers into
// a slice of 0 or more event containers.
type EventProcessor = func(
	*sql.Tx, // sourceTx
	*sql.Tx, // targetTx
	[]messages.MessageContainer,
) ([]messages.MessageContainer, error)

// ProcessEvents queries the sourceDB event table, processes
// each event and perists the output to the targetDB event table.
// Seq (sequence)
//   - Sequences MAY be different between sourceDB and targetDB
//   - Sequences WILL be in the same order on sourceDB and targetDB
// Checkpointing
//   - checkpoints are persisted to the targetDB and sourceDB in that order
//   - the checkpoint value is the last sourceDB Seq processed
//   - the targetDB transaction is committed _before_ the sourceDB transaction
//   - on the next tick, events with a greater Seq than the greatest of the sourceDB and targetDB
//      checkpoint Seq are processed.
//   - the targetDB checkpoint serves as a _fencing token_ (see Kleppmann, Martin, Designing Data
//		Intensive Applictions, O'Reilly 2017 pp676-680,
//		or, https://martin.kleppmann.com/2016/02/08/how-to-do-distributed-locking.html)
func ProcessEvents(
	processor EventProcessor, sourceDB *sql.DB, targetDB *sql.DB, checkpointKey string,
) error {
	sourceTx, err := sourceDB.Begin()
	if err != nil {
		return err
	}
	defer sourceTx.Rollback()
	targetTx, err := targetDB.Begin()
	if err != nil {
		return err
	}
	defer targetTx.Rollback()

	seqCheckpoint, err := fetchSeqCheckpoint(sourceTx, targetTx, checkpointKey)
	if err != nil {
		return err
	}

	eventContainers, seqCheckpoint, err := fetchEventContainers(sourceTx, seqCheckpoint)
	if err != nil {
		return err
	}

	targetEventContainers, err := processor(sourceTx, targetTx, eventContainers)
	if err != nil {
		return err
	}

	if err := persistEventContainers(targetTx, targetEventContainers); err != nil {
		return err
	}

	if err := persistTargetSeqCheckpoint(targetTx, seqCheckpoint, checkpointKey); err != nil {
		return err
	}

	if err := targetTx.Commit(); err != nil {
		return err
	}

	if err := persistSourceSeqCheckpoint(sourceTx, seqCheckpoint, checkpointKey); err != nil {
		return err
	}

	return sourceTx.Commit()
}

// fetchSeqCheckpoint fetches the highest checkpoint from the source
// and the target databases. The target can be higher in the situation where
// this module fails after commiting to the target but before updating the
// source checkpoint, or, in the case of delievery after timeout. The
// targetSeqCheckpoint serves as a fencing token.
func fetchSeqCheckpoint(sourceTx *sql.Tx, targetTx *sql.Tx, checkpointKey string) (uint64, error) {
	sourceSeqCheckpoint, err := fetchSourceSeqCheckpoint(sourceTx, checkpointKey)
	if err != nil {
		return 0, err
	}
	targetSeqCheckpoint, err := fetchTargetSeqCheckpoint(targetTx, checkpointKey)
	if err != nil {
		return 0, err
	}
	seqCheckpoint := sourceSeqCheckpoint
	if targetSeqCheckpoint > sourceSeqCheckpoint {
		seqCheckpoint = targetSeqCheckpoint
	}

	return seqCheckpoint, nil
}

func fetchSourceSeqCheckpoint(sourceTx *sql.Tx, checkpointKey string) (uint64, error) {
	var (
		sourceSeqCheckpoint uint64
	)
	err := sourceTx.QueryRow(`
		SELECT seq FROM event_processor_source_checkpoints
		WHERE id = $1
		FOR UPDATE;
	`, checkpointKey).Scan(&sourceSeqCheckpoint)
	if err != nil {
		if err == sql.ErrNoRows {
			stmt, err := sourceTx.Prepare(`
				INSERT INTO event_processor_source_checkpoints (
				  id, seq
				) VALUES ($1, 0);
			`)
			if err != nil {
				return 0, err
			}
			if _, err := stmt.Exec(checkpointKey); err != nil {
				return 0, err
			}
			return fetchSourceSeqCheckpoint(sourceTx, checkpointKey)
		}
		return 0, err
	}
	return sourceSeqCheckpoint, nil
}

func persistSourceSeqCheckpoint(sourceTx *sql.Tx, seqCheckpoint uint64, checkpointKey string) error {
	stmt, err := sourceTx.Prepare(`
		UPDATE event_processor_source_checkpoints
		SET seq = $1
		WHERE id = $2;
	`)
	if err != nil {
		return err
	}
	_, err = stmt.Exec(seqCheckpoint, checkpointKey)
	return err
}

func fetchTargetSeqCheckpoint(targetTx *sql.Tx, checkpointKey string) (uint64, error) {
	var (
		targetSeqCheckpoint uint64
	)
	err := targetTx.QueryRow(`
		SELECT seq FROM event_processor_target_checkpoints
		WHERE id = $1
		FOR UPDATE;
	`, checkpointKey).Scan(&targetSeqCheckpoint)
	if err != nil {
		if err == sql.ErrNoRows {
			stmt, err := targetTx.Prepare(`
				INSERT INTO event_processor_target_checkpoints (
				  id, seq
				) VALUES ($1, 0);
			`)
			if err != nil {
				return 0, err
			}
			if _, err := stmt.Exec(checkpointKey); err != nil {
				return 0, err
			}
			return fetchTargetSeqCheckpoint(targetTx, checkpointKey)
		}
		return 0, err
	}
	return targetSeqCheckpoint, nil
}

func persistTargetSeqCheckpoint(targetTx *sql.Tx, seqCheckpoint uint64, checkpointKey string) error {
	checkpointStmt, err := targetTx.Prepare(`
		UPDATE event_processor_target_checkpoints
		SET seq = $1
		WHERE id = $2;
	`)
	if err != nil {
		return err
	}
	if _, err := checkpointStmt.Exec(seqCheckpoint, checkpointKey); err != nil {
		return err
	}
	return nil
}

func fetchEventContainers(
	sourceTx *sql.Tx,
	seqCheckpoint uint64,
) ([]messages.MessageContainer, uint64, error) {
	var (
		eventContainers []messages.MessageContainer
	)
	eventStmt, err := sourceTx.Prepare(`
		SELECT seq, message
		FROM events_shard0000
		WHERE seq > $1
		ORDER BY seq
		LIMIT 100;
	`)
	if err != nil {
		return eventContainers, seqCheckpoint, err
	}
	defer eventStmt.Close()
	rows, err := eventStmt.Query(seqCheckpoint)
	if err != nil {
		return eventContainers, seqCheckpoint, err
	}
	defer rows.Close()

	for rows.Next() {
		container := messages.MessageContainer{}
		var bContainer []byte
		rows.Scan(&seqCheckpoint, &bContainer)
		err := proto.Unmarshal(bContainer, &container)
		if err != nil {
			return eventContainers, seqCheckpoint, err
		}
		eventContainers = append(eventContainers, container)
	}

	return eventContainers, seqCheckpoint, nil
}

func persistEventContainers(
	targetTx *sql.Tx,
	containers []messages.MessageContainer,
) error {
	eventStmt, err := targetTx.Prepare(`
		INSERT INTO events_shard0000 (
			aggregate_root_id, message_type, message
		) VALUES ($1, $2, $3);
	`)
	if err != nil {
		return err
	}

	for _, container := range containers {
		bContainer, err := proto.Marshal(&container)
		if err != nil {
			return err
		}
		_, err = eventStmt.Exec(container.AggregateRootID, container.MessageType, bContainer)
		if err != nil {
			return err
		}
	}

	return nil
}
