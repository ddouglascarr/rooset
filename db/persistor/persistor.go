package persistor

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	proto "github.com/golang/protobuf/proto"
	"github.com/google/uuid"
)

// PersistMessageContainer just for testing
func PersistMessageContainer(db *sql.DB, container *messages.MessageContainer) error {
	containerBin, err := proto.Marshal(container)
	if err != nil {
		return err
	}
	stmpPersist, err := db.Prepare(`
		INSERT INTO events_shard0000
		    (aggregate_root_id, message_type, message)
		VALUES ($1, $2, $3)
	`)
	if err != nil {
		return err
	}

	if _, err := stmpPersist.Exec(container.AggregateRootID, container.MessageType, containerBin); err != nil {
		return err
	}
	return nil
}

// BuildEvtContainer foobar
func BuildEvtContainer() (messages.MessageContainer, error) {
	evt0 := messages.UnitCreatedEvent{
		UnitID:           uuid.New().String(),
		RequesterID:      uuid.New().String(),
		Name:             "Foo Unit",
		Description:      "The Foo Unit",
		URLParameterName: "pants-unit",
	}

	evt0Container, err := messages.WrapMessage(&evt0)
	return evt0Container, err
}
