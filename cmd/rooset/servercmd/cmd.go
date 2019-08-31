package servercmd

import (
	"github.com/ddouglascarr/rooset/server"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

type serverCmd struct {
}

// NewServerCmd command builder
func NewServerCmd() (cmdy.Command, cmdy.Init) {
	return &serverCmd{}, nil
}

var _ cmdy.Command = &serverCmd{}

func (t *serverCmd) Synopsis() string { return "Starts a HTTP server" }

func (t *serverCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *serverCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *serverCmd) Run(ctx cmdy.Context) error {
	err := server.ListenAndServe()
	if err != nil {
		panic(err)
	}

	return nil
}
