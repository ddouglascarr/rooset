package queryappcmd

import (
	"fmt"

	"github.com/ddouglascarr/rooset/github"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewQueryAppCmd command builder
func NewQueryAppCmd() (cmdy.Command, cmdy.Init) {
	return &queryAppCmd{}, nil
}

type queryAppCmd struct{}

var _ cmdy.Command = &queryAppCmd{}

func (t *queryAppCmd) Synopsis() string { return "Query the Github App endpoint" }

func (t *queryAppCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *queryAppCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *queryAppCmd) Run(ctx cmdy.Context) error {
	resp, err := github.QueryApp()
	if err != nil {
		return err
	}

	fmt.Printf(resp)
	return nil
}
