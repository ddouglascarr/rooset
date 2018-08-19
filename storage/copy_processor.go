package storage

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
)

func CopyEventProcessor(
	_ *sql.Tx,
	_ *sql.Tx,
	containers []messages.MessageContainer,
) ([]messages.MessageContainer, error) {
	return containers, nil
}
