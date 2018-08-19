package main

import (
	"database/sql"
	"flag"
	"log"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage/persistor"
	_ "github.com/lib/pq"
)

func persist(evtJSON string) error {
	container, err := messages.UnmarshalJSONMessageContainer([]byte(evtJSON))
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

	persistor.PersistMessageContainer(db, container)
	return nil

}

func main() {
	var evtJSON string
	flag.StringVar(&evtJSON, "container", "", "JSON formatted message container")
	flag.Parse()
	if err := persist(evtJSON); err != nil {
		log.Fatal(err)
	}
}
