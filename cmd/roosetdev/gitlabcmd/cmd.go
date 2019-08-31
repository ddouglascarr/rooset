package gitlabcmd

import (
	"github.com/ddouglascarr/rooset/gitlabclient"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewGitlabClientCmd builder
func NewGitlabClientCmd() (cmdy.Command, cmdy.Init) {
	return &gitlabClientCmd{}, nil
}

type gitlabClientCmd struct{}

var _ cmdy.Command = &gitlabClientCmd{}

func (t *gitlabClientCmd) Synopsis() string {
	return "Test gitlab connection"
}

func (t *gitlabClientCmd) Flags() *cmdy.FlagSet {
	fs := cmdy.NewFlagSet()
	return fs
}

func (t *gitlabClientCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *gitlabClientCmd) Run(ctx cmdy.Context) error {
	gitlabclient.Ping()
	return nil
}
