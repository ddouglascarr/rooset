package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/aggregates"
	"github.com/ddouglascarr/rooset/messages"
)

const messageProcessorLimit = 100

// MessageProcessor transforms a slice of messages into another
// slice of 0 or more messages
type MessageProcessor = func(
	*sql.Tx, // sourceTx
	*sql.Tx, // targetTx
	[]messages.Message,
) ([]messages.Message, error)

// ProcessMessages queries the sourceDB event table, processes
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
func ProcessMessages(
	processor MessageProcessor, sourceDB *sql.DB, targetDB *sql.DB, checkpointKey string,
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

	sourceMessages, seqCheckpoint, err := FetchMessagesSinceSeq(
		sourceTx,
		seqCheckpoint,
		messageProcessorLimit,
	)
	if err != nil {
		return err
	}

	targetMessages, err := processor(sourceTx, targetTx, sourceMessages)
	if err != nil {
		return err
	}

	if err := PersistMessages(targetTx, targetMessages); err != nil {
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

// ExecuteCommand is the primary command dispatcher. All commands should
// be issued to this function, it will generate one or less events
// to be persisted on success.
// It returns:
// - a rejection reason string, nil if the command was accepted
// - an error which is not nil if there was a system error
func ExecuteCommand(commandDB *sql.DB, cmd messages.Message) error {
	aRField, err := messages.GetAggregateRootField(cmd)
	if err != nil {
		return err
	}

	aRID, err := messages.GetAggregateRootID(cmd)
	if err != nil {
		return err
	}

	tx, err := commandDB.Begin()
	if err != nil {
		return err
	}
	defer tx.Rollback()

	fetchAggregate := func(aRID string, aggregate aggregates.Aggregate) error {
		return FetchAggregate(tx, aRID, aggregate)
	}

	evt, err := aggregates.HandleCommand(fetchAggregate, aRField, aRID, cmd)
	if err != nil {
		return err
	}

	if evt != nil {
		err := PersistMessages(tx, []messages.Message{evt})
		if err != nil {
			return err
		}
	}
	err = tx.Commit()
	return err
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
