package sync

import (
	"fmt"
	"strings"

	"github.com/ddouglascarr/rooset/lfclient"
	"github.com/xanzy/go-gitlab"
)

type Situation string

var (
	CreateInitiative          = Situation("CreateInitiative")
	CreateCompetingInitiative = Situation("CreateCompetingInitiative")
	RebaseToIssue             = Situation("RebaseToIssue")
	ActiveIssue               = Situation("ActiveIssue")
	ClosePullRequest          = Situation("ClosePullRequest")
	Closed                    = Situation("Closed")
	Invalid                   = Situation("Invalid")
)

func Classify(pullRequest *gitlab.MergeRequest, initiative *lfclient.Initiative) (
	Situation, error) {
	switch {
	case initiative == nil:
		if strings.HasPrefix(pullRequest.TargetBranch, "lf-area-") {
			// first initiative in new issue
			return CreateInitiative, nil
		}
		if strings.HasPrefix(pullRequest.TargetBranch, "lf-issue-") {
			// competing initiative
			return CreateCompetingInitiative, nil
		}
		return Invalid, nil
	case !initiative.IsActive() && pullRequest.State != "closed":
		return ClosePullRequest, nil
	case !initiative.IsActive() && pullRequest.State == "closed":
		return Closed, nil
	case pullRequest.TargetBranch == "master":
		return RebaseToIssue, nil
	case pullRequest.TargetBranch == buildTargetBranchName(*initiative):
		return ActiveIssue, nil
	default:
		return Invalid, nil
	}
}

func buildTargetBranchName(initiative lfclient.Initiative) string {
	return fmt.Sprintf("lf-issue-%d", initiative.IssueID)
}

// RebaseNewIssueInitiative creates a new issue branch, and rebases the initiative PR against it
func RebaseNewIssueInitiative(projectID string, pRID int, issueID int) error {
	// TODO implement
	return nil
}
