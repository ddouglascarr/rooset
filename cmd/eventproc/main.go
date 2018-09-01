package main

import (
	"database/sql"
	"log"

	"github.com/ddouglascarr/rooset/projections"
	"github.com/ddouglascarr/rooset/storage"
	_ "github.com/lib/pq"

	//	"runtime/debug"
	"time"
)

func processLoop() error {
	truthDB, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_0 host=localhost sslmode=disable",
	)
	if err != nil {
		return err
	}
	defer truthDB.Close()

	projectionDB, err := sql.Open(
		"postgres",
		"user=postgres dbname=rooset_test_projections_0 host=localhost sslmode=disable",
	)
	if err != nil {
		return err
	}
	defer projectionDB.Close()

	for {
		if err := storage.ProcessMessages(
			storage.CopyMessageProcessor,
			truthDB,
			projectionDB,
			"shard0000",
		); err != nil {
			return err
		}
		if err := storage.ProcessMessages(
			projections.UnitProjectionEventProcessor,
			truthDB,
			projectionDB,
			"projections0000",
		); err != nil {
			return err
		}
		time.Sleep(5000 * time.Millisecond)
	}
}

func main() {
	if err := processLoop(); err != nil {
		log.Fatal(err)
	}
}
