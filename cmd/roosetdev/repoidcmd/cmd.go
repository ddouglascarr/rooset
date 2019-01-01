package repoidcmd

import (
	"fmt"

	"github.com/ddouglascarr/rooset/github"
	"github.com/shabbyrobe/cmdy"
	"github.com/shabbyrobe/cmdy/args"
)

// NewRepoIDCmd builder
func NewRepoIDCmd() (cmdy.Command, cmdy.Init) {
	return &repoIDCmd{}, nil
}

type repoIDCmd struct {
	Owner          string
	Name           string
	InstallationID int64
}

var _ cmdy.Command = &repoIDCmd{}

func (t *repoIDCmd) Synopsis() string { return "Fetch the GraphQL ID of a github repository" }

func (t *repoIDCmd) Flags() *cmdy.FlagSet {
	fs := cmdy.NewFlagSet()
	fs.StringVar(&t.Owner, "owner", "", "Owner of the repository")
	fs.StringVar(&t.Name, "name", "", "Name of the repository")
	fs.Int64Var(&t.InstallationID, "installationid", 0, "InstallationID")
	return fs
}

func (t *repoIDCmd) Args() *args.ArgSet {
	return args.NewArgSet()
}

func (t *repoIDCmd) Run(ctx cmdy.Context) error {
	ID, err := github.FetchRepoID(t.InstallationID, t.Owner, t.Name)
	if err != nil {
		return err
	}
	fmt.Printf(ID)
	return nil
}
