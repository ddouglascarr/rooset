package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
)

// CopyMessageProcessor returns whatever messages it's given, for copying
// events from the sourceDB to the targetDB
func CopyMessageProcessor(
	_ *sql.Tx,
	_ *sql.Tx,
	msgs []messages.Message,
) ([]messages.Message, error) {
	return msgs, nil
}
