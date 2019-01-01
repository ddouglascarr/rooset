package githubapptkcmd

import (
	"fmt"

	"github.com/ddouglascarr/rooset/github"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewGithubAppTkCmd builder
func NewGithubAppTkCmd() (cmdy.Command, cmdy.Init) {
	return &githubAppTkCmd{}, nil
}

type githubAppTkCmd struct{}

var _ cmdy.Command = &githubAppTkCmd{}

func (t *githubAppTkCmd) Synopsis() string { return "Generate a Github App JWT" }

func (t *githubAppTkCmd) Flags() *cmdy.FlagSet {
	return cmdy.NewFlagSet()
}

func (t *githubAppTkCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *githubAppTkCmd) Run(ctx cmdy.Context) error {
	tk, err := github.NewGithubAppJWT()
	if err != nil {
		return err
	}
	fmt.Printf(tk)

	return nil
}
