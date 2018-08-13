package main

import (
	"database/sql"
	"github.com/ddouglascarr/rooset/db/eventproc"
	"github.com/ddouglascarr/rooset/projections"
	_ "github.com/lib/pq"
	"log"
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
		if err := eventproc.ProcessEvents(
			eventproc.CopyEventProcessor,
			truthDB,
			projectionDB,
			"shard0000",
		); err != nil {
			return err
		}
		if err := eventproc.ProcessEvents(
			projections.UnitProjectionEventProcessor,
			truthDB,
			projectionDB,
			"projections0000",
		); err != nil {
			return err
		}
		// if err := eventproc.ProcessEvents(
		// 	projections.ProcessProjectionEvents,
		// 	projectionDB,
		// 	projectionDB,
		// 	"projections0000",
		// ); err != nil {
		// 	debug.PrintStack()
		// 	return err
		// }
		time.Sleep(5000 * time.Millisecond)
	}
}

func main() {
	if err := processLoop(); err != nil {
		log.Fatal(err)
	}
}
