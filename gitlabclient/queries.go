package gitlabclient

import (
	"github.com/xanzy/go-gitlab"
)

// GetMergeRequest returns merge request
func GetMergeRequest(projectID string, mergeRequestID int) (*gitlab.MergeRequest, error) {
	git := gitlab.NewClient(nil, "y816jUs-hhV8sokfR_Fe")
	git.SetBaseURL("http://localhost:9080/api/v4")

	mr, _, err := git.MergeRequests.GetMergeRequest(
		projectID, mergeRequestID, &gitlab.GetMergeRequestsOptions{},
	)
	if err != nil {
		return nil, err
	}
	return mr, nil
}
