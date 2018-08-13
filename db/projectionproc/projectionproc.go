package projectionproc

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
)

// FetchProjection fetches a projection from the database, and creates an
// empty projection if it does not exist
func FetchProjection(
	tx *sql.Tx,
	projection proto.Message,
	ID string,
) error {

	container, err := FetchProjectionContainer(tx, messages.GetMessageType(projection), ID)
	if err != nil {
		return err
	}
	err = proto.Unmarshal(container.Payload, projection)
	return err
}

// FetchProjectionContainer gets the latest incident of a container
func FetchProjectionContainer(tx *sql.Tx, messageType string, ID string) (messages.MessageContainer, error) {
	var bContainer []byte
	container := messages.MessageContainer{}

	stmt, err := tx.Prepare(`
		SELECT message FROM events_shard0000
		WHERE message_type = $1 AND aggregate_root_id = $2
		ORDER BY seq DESC LIMIT 1;
	`)
	if err != nil {
		return container, err
	}

	if err := stmt.QueryRow(messageType, ID).Scan(&bContainer); err != nil {
		if err == sql.ErrNoRows {
			// no projection exists yet. Use the empty one
			return container, nil
		}
		return container, err
	}

	if err = proto.Unmarshal(bContainer, &container); err != nil {
		return container, err
	}

	return container, nil
}
