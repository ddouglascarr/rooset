package githubinstallationtkcmd

import (
	"fmt"

	"github.com/ddouglascarr/rooset/github"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewGithubAppTkCmd builder
func NewGithubAppTkCmd() (cmdy.Command, cmdy.Init) {
	return &githubInstallationTkCmd{}, nil
}

type githubInstallationTkCmd struct{ InstallationID int64 }

var _ cmdy.Command = &githubInstallationTkCmd{}

func (t *githubInstallationTkCmd) Synopsis() string {
	return "Fetch a Github installation auth token"
}

func (t *githubInstallationTkCmd) Flags() *cmdy.FlagSet {
	fs := cmdy.NewFlagSet()
	fs.Int64Var(&t.InstallationID, "installationid", 0, "Github installation id")
	return fs
}

func (t *githubInstallationTkCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *githubInstallationTkCmd) Run(ctx cmdy.Context) error {
	tk, err := github.FetchInstallationTk(t.InstallationID)
	if err != nil {
		return err
	}
	fmt.Printf(tk)

	return nil
}
