package installationscmd

import (
	"encoding/json"
	"fmt"

	"github.com/ddouglascarr/rooset/github"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewInstallationsCmd builder
func NewInstallationsCmd() (cmdy.Command, cmdy.Init) {
	return &installationsCmd{}, nil
}

type installationsCmd struct{}

var _ cmdy.Command = &installationsCmd{}

func (t *installationsCmd) Synopsis() string { return "List all app installations" }

func (t *installationsCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *installationsCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *installationsCmd) Run(ctx cmdy.Context) error {
	installations, err := github.FetchAllInstallations()
	if err != nil {
		return err
	}

	out, err := json.Marshal(installations)
	if err != nil {
		return err
	}
	fmt.Printf(string(out))

	return nil
}
