package execcmd

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/pkg/errors"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewExecCmd command builder
func NewExecCmd() (cmdy.Command, error) {
	return &execCmd{}, nil
}

type execCmd struct {
	JSONMsg string
}

var _ cmdy.Command = &execCmd{}

func (t *execCmd) Synopsis() string { return "Executes a command" }

func (t *execCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *execCmd) Args() *args.ArgSet {
	as := args.NewArgSet()
	as.String(&t.JSONMsg, "msg", "JSON command message")
	return as
}

func (t *execCmd) Run(ctx cmdy.Context) error {
	msg, err := messages.UnmarshalJSONMessage([]byte(t.JSONMsg))
	if err != nil {
		return errors.Wrap(err, "rooset: Invalid JSON")
	}

	db, err := sql.Open("postgres", "user=postgres dbname=rooset_test_0 host=localhost sslmode=disable")
	if err != nil {
		return errors.Wrap(err, "rooset: failed to connect to db")
	}
	defer db.Close()
	if err := db.Ping(); err != nil {
		return errors.Wrap(err, "rooset: failed to connect to db")
	}

	if err := storage.ExecuteCommand(db, msg); err != nil {
		return errors.Wrap(err, "rooset: failed to execute command")
	}

	return nil
}
