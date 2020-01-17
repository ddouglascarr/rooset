package main

import (
	"context"
	"os"

	"github.com/ddouglascarr/rooset/docsvc"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/arg"
)

type docsvcCmd struct{}

func newDocsvcCmd() cmdy.Command {
	return &docsvcCmd{}
}

func (t *docsvcCmd) Synopsis() string {
	return "Server for validating and saving rooset document to a git server"
}

func (t *docsvcCmd) Configure(flags *cmdy.FlagSet, args *arg.ArgSet) {}

func (t *docsvcCmd) Run(ctx cmdy.Context) error {
	docsvc.Run()
	return nil
}

func run() error {
	mainGroupBuilder := func() cmdy.Command {
		return cmdy.NewGroup(
			"docsvc",
			cmdy.Builders{
				"docsvc": newDocsvcCmd,
			},
		)
	}

	return cmdy.Run(context.Background(), os.Args[1:], mainGroupBuilder)
}

func main() {
	if err := run(); err != nil {
		cmdy.Fatal(err)
	}
}
