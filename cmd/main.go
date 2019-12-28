package main

import (
	"context"
	"fmt"
	"os"

	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/arg"
)

type gitServiceCmd struct{}

func newGitServiceCmd() cmdy.Command {
	return &gitServiceCmd{}
}

func (t *gitServiceCmd) Synopsis() string {
	return "Server for validating and saving rooset document to a git server"
}

func (t *gitServiceCmd) Configure(flags *cmdy.FlagSet, args *arg.ArgSet) {}

func (t *gitServiceCmd) Run(ctx cmdy.Context) error {
	fmt.Println("Hello Cmdy World")
	return nil
}

func run() error {
	mainGroupBuilder := func() cmdy.Command {
		return cmdy.NewGroup(
			"Git Service",
			cmdy.Builders{
				"gitservice": newGitServiceCmd,
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
