package main

import (
	"database/sql"
	"flag"
	"fmt"
	"os"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	_ "github.com/lib/pq"
	"github.com/pkg/errors"
)

func persist(jSONMsg string) error {
	msg, err := messages.UnmarshalJSONMessage([]byte(jSONMsg))
	if err != nil {
		return err
	}

	db, err := sql.Open("postgres", "user=postgres dbname=rooset_test_0 host=localhost sslmode=disable")
	if err != nil {
		return err
	}
	defer db.Close()
	if err := db.Ping(); err != nil {
		return err
	}

	tx, err := db.Begin()
	if err != nil {
		return err
	}
	defer tx.Rollback()

	storage.PersistMessages(tx, []messages.Message{msg})
	err = tx.Commit()
	if err != nil {
		return errors.Wrap(err, "failed to commit")
	}

	return nil
}

func main() {
	var jSONMsg string
	flag.StringVar(&jSONMsg, "container", "", "JSON formatted message container")
	flag.Parse()
	if err := persist(jSONMsg); err != nil {
		fmt.Printf("FATAL: %+v\n", err)
		os.Exit(1)
	}
}
