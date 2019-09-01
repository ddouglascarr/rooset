package classifycmd

import (
	"log"

	"github.com/ddouglascarr/rooset/gitlabclient"
	"github.com/ddouglascarr/rooset/lfclient"
	"github.com/ddouglascarr/rooset/sync"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewClassifyCmd builder
func NewClassifyCmd() (cmdy.Command, cmdy.Init) {
	return &classifyCmd{}, nil
}

type classifyCmd struct {
	InitiativeID  int
	PullRequestID int
}

var _ cmdy.Command = &classifyCmd{}

func (t *classifyCmd) Synopsis() string {
	return "Test gitlab connection"
}

func (t *classifyCmd) Flags() *cmdy.FlagSet {
	fs := cmdy.NewFlagSet()
	fs.IntVar(&t.InitiativeID, "initiative", 0, "lf initiative id")
	fs.IntVar(&t.PullRequestID, "pr", 0, "pull request id")
	return fs
}

func (t *classifyCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *classifyCmd) Run(ctx cmdy.Context) error {
	mergeRequest, err := gitlabclient.GetMergeRequest("determined_poitras/test-public-1", t.PullRequestID)
	if err != nil {
		log.Fatal(err)
	}

	var initiative *lfclient.Initiative
	if t.InitiativeID != 0 {
		initiative, err = lfclient.GetInitiative(t.InitiativeID)
		if err != nil {
			log.Fatal(err)
		}
	}

	situation, err := sync.Classify(mergeRequest, initiative)
	if err != nil {
		log.Fatal(err)
	}
	log.Println(situation)

	return nil
}
