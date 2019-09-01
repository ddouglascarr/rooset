package gitlabcmd

import (
	"log"

	"github.com/ddouglascarr/rooset/gitlabclient"
	"github.com/ddouglascarr/rooset/lfclient"
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
	mergeRequest, err := gitlabclient.GetMergeRequest("determined_poitras/test-public-1", 1)
	if err != nil {
		log.Fatal(err)
	}
	log.Println(mergeRequest.Title)

	initiative, err := lfclient.GetInitiative(1)
	if err != nil {
		log.Fatal(err)
	}

	log.Println(string(initiative.IssueState), initiative.Name)
	return nil
}
