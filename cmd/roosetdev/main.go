package main

import (
	"context"
	"os"

	"github.com/ddouglascarr/rooset/cmd/roosetdev/execcmd"
	"github.com/ddouglascarr/rooset/cmd/roosetdev/queryappcmd"
	"github.com/ddouglascarr/rooset/cmd/roosetdev/saveevtcmd"
	_ "github.com/lib/pq"
	"github.com/shabbyrobe/cmdy"
)

func run() error {
	bld := func() (cmdy.Command, cmdy.Init) {
		return cmdy.NewGroup(
			"My command group",
			cmdy.Builders{
				"saveevt":  saveevtcmd.NewSaveEvtCmd,
				"exec":     execcmd.NewExecCmd,
				"queryapp": queryappcmd.NewQueryAppCmd,
			},
		), nil
	}
	return cmdy.Run(context.Background(), os.Args[1:], bld)
}

func main() {
	if err := run(); err != nil {
		cmdy.Fatal(err)
	}
}
