package projproccmd

import (
	"database/sql"
	"time"

	"github.com/ddouglascarr/rooset/projections"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

type projProcCmd struct {
}

// NewProjProcCmd command builder
func NewProjProcCmd() (cmdy.Command, error) {
	return &projProcCmd{}, nil
}

var _ cmdy.Command = &projProcCmd{}

func (t *projProcCmd) Synopsis() string { return "Executes a command" }

func (t *projProcCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *projProcCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *projProcCmd) Run(ctx cmdy.Context) error {
	commandDB, err := sql.Open(
		"postgres",
		"user=postgres  dbname=rooset_test_0 host=localhost sslmode=disable",
	)
	if err != nil {
		return err
	}
	defer commandDB.Close()

	queryDB, err := sql.Open(
		"postgres",
		"user=postgres  dbname=rooset_test_projections_0 host=localhost sslmode=disable",
	)
	if err != nil {
		return err
	}
	defer queryDB.Close()

	for {
		if err := storage.ProcessMessages(
			storage.CopyMessageProcessor,
			commandDB,
			queryDB,
			"shard0000",
		); err != nil {
			return err
		}
		if err := storage.ProcessMessages(
			projections.UnitProjectionEventProcessor,
			commandDB,
			queryDB,
			"projections0000",
		); err != nil {
			return err
		}
		time.Sleep(1000 * time.Millisecond)
	}
}
