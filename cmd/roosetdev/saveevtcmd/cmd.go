package saveevtcmd

import (
	"database/sql"

	"github.com/ddouglascarr/rooset/messages"
	"github.com/ddouglascarr/rooset/storage"
	"github.com/pkg/errors"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewSaveEvtCmd command builder
func NewSaveEvtCmd() (cmdy.Command, error) {
	return &saveEvtCmd{}, nil
}

type saveEvtCmd struct {
	JSONMsg string
}

var _ cmdy.Command = &saveEvtCmd{}

func (t *saveEvtCmd) Synopsis() string { return "Persists an event to the command event stream" }

func (t *saveEvtCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *saveEvtCmd) Args() *args.ArgSet {
	as := args.NewArgSet()
	as.String(&t.JSONMsg, "msg", "JSON event message")
	return as
}

func (t *saveEvtCmd) Run(ctx cmdy.Context) error {
	msg, err := messages.UnmarshalJSONMessage([]byte(t.JSONMsg))
	if err != nil {
		return errors.Wrap(err, "rooset: Invalid JSON")
	}

	db, err := sql.Open("postgres", "user=postgres  dbname=rooset_test_0 host=localhost sslmode=disable")
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
