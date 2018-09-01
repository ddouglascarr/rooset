package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/golang/protobuf/proto"
)

// PersistMessages persists all messages to the database.
func PersistMessages(tx *sql.Tx, msgs []messages.Message) error {
	stmt, err := tx.Prepare(`
		INSERT INTO events_shard0000 (
			aggregate_root_id, message_type, message
		) VALUES ($1, $2, $3);
	`)
	if err != nil {
		return err
	}
	defer stmt.Close()

	for _, msg := range msgs {
		messageType := messages.GetMessageType(msg)
		aRID, err := messages.GetAggregateRootID(msg)
		if err != nil {
			return err
		}
		bMsg, err := messages.MarshalBMessage(msg)
		if err != nil {
			return err
		}

		_, err = stmt.Exec(aRID, messageType, bMsg)
		if err != nil {
			return err
		}
	}

	return nil
}

// FetchMessagesSinceSeq fetches messages from a above a sequence to a limit
func FetchMessagesSinceSeq(
	tx *sql.Tx,
	seqCheckpoint uint64,
	limit uint,
) ([]messages.Message, uint64, error) {
	var msgs []messages.Message
	stmt, err := tx.Prepare(`
		SELECT seq, message_type, message
		FROM events_shard0000
		WHERE seq > $1
		ORDER BY seq
		LIMIT $2;
	`)
	if err != nil {
		return msgs, seqCheckpoint, err
	}
	defer stmt.Close()

	rows, err := stmt.Query(seqCheckpoint, limit)
	if err != nil {
		return msgs, seqCheckpoint, err
	}
	defer rows.Close()

	for rows.Next() {
		var (
			messageType string
			bMsg        []byte
		)
		rows.Scan(
			&seqCheckpoint,
			&messageType,
			&bMsg,
		)
		msg, err := messages.UnmarshalBMessage(messageType, bMsg)
		if err != nil {
			return msgs, seqCheckpoint, err
		}
		msgs = append(msgs, msg)
	}

	return msgs, seqCheckpoint, nil

}

// FetchProjection fetches a projection from the database
// if there is no projection there, then it's a new projection
// It does not set the ARID on the projection container. I'm unsure if
// that is a good call.
// TODO: maybe get the ARID from the passed in proj, so the caller has
// to have already initialized the projection with an ARID. Harder to bugger it up
// that way.
func FetchProjection(
	tx *sql.Tx,
	proj messages.Message,
	ID string,
) error {
	stmt, err := tx.Prepare(`
		SELECT message FROM events_shard0000
		WHERE message_type = $1 AND aggregate_root_id = $2
		ORDER BY seq DESC LIMIT 1;
	`)
	if err != nil {
		return err
	}

	var bProj []byte

	if err := stmt.QueryRow(
		messages.GetMessageType(proj),
		ID,
	).Scan(
		&bProj,
	); err != nil {
		if err == sql.ErrNoRows {
			// no projection exists yet. Use the empty one
			return nil
		}
		return err
	}

	err = proto.Unmarshal(bProj, proj)
	if err != nil {
		return err
	}

	return nil
}
