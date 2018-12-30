package projproccmd

import (
	"database/sql"
	"fmt"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/projections"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

type projProcCmd struct {
}

// NewProjProcCmd command builder
func NewProjProcCmd() (cmdy.Command, cmdy.Init) {
	return &projProcCmd{}, nil
}

var _ cmdy.Command = &projProcCmd{}

func (t *projProcCmd) Synopsis() string { return "Starts a projection processor" }

func (t *projProcCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *projProcCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *projProcCmd) Run(ctx cmdy.Context) error {
	commandDB, err := sql.Open(
		"postgres",
		fmt.Sprintf(
			"user=%s dbname=%s host=%s sslmode=disable",
			conf.DB.User, conf.DB.CmdName, conf.DB.CmdHost,
		),
	)
	if err != nil {
		return err
	}
	defer commandDB.Close()

	queryDB, err := sql.Open(
		"postgres",
		fmt.Sprintf(
			"user=%s dbname=%s host=%s sslmode=disable",
			conf.DB.User, conf.DB.QueryName, conf.DB.QueryHost,
		),
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
			projections.UnitProjectionEvtProcessor,
			commandDB,
			queryDB,
			"projections0000",
		); err != nil {
			return err
		}
		time.Sleep(1000 * time.Millisecond)
	}
}
