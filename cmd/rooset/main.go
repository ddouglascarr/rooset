package main

import (
	"context"
	"os"

	"github.com/ddouglascarr/rooset/cmd/rooset/projproccmd"
	"github.com/ddouglascarr/rooset/cmd/rooset/servercmd"
	_ "github.com/lib/pq"
	"github.com/shabbyrobe/cmdy"
)

func run() error {
	bld := func() (cmdy.Command, cmdy.Init) {
		return cmdy.NewGroup(
			"My command group",
			cmdy.Builders{
				"server":   servercmd.NewServerCmd,
				"projproc": projproccmd.NewProjProcCmd,
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
