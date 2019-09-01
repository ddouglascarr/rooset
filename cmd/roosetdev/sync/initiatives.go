package sync

import (
	"fmt"

	"github.com/ddouglascarr/rooset/lfclient"
	"github.com/xanzy/go-gitlab"
)

type Situation string

var (
	CreateInitiative = Situation("CreateInitiative")
	RebaseToIssue    = Situation("RebaseToIssue")
	ActiveIssue      = Situation("ActiveIssue")
	Invalid          = Situation("Invalid")
)

func Classify(pullRequest *gitlab.MergeRequest, initiative *lfclient.Initiative) (
	Situation, error) {
	switch {
	case initiative == nil:
		if pullRequest.TargetBranch == "master" {
			return CreateInitiative, nil
		}
		return Invalid, nil
	case pullRequest.TargetBranch == "master" && initiative.IsActive():
		return RebaseToIssue, nil
	case pullRequest.TargetBranch == buildTargetBranchName(*initiative) && initiative.IsActive():
		return ActiveIssue, nil
	default:
		return Invalid, nil
	}
}

func buildTargetBranchName(initiative lfclient.Initiative) string {
	return fmt.Sprintf("lf-issue-%d", initiative.IssueID)
}
