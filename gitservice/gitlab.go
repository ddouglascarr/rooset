package gitservice

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/pkg/errors"
)

//SHA is the sha of a git commit
type SHA string

const (
	gitlabAuthTk = "LqzcHTKxhXCATKmzzo_h" // TODO: get from config
)

type createCommitGitlabResp struct {
	ID string `json:"id"`
}

func createCommit(
	repositoryName string,
	startBranch string,
	branch string,
	actions []Action,
) (*GitRecord, error) {
	url := fmt.Sprintf("https://gitlab.com/api/v4/projects/ddouglascarr%%2F%s/repository/commits", repositoryName)
	req, err := http.NewRequest("POST", url, nil)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: commit request failed")
	}
	req.Header.Add("Authorization", fmt.Sprintf("Bearer: %s", gitlabAuthTk))
	req.Header.Add("Accept", "application/json")

	client := &http.Client{
		Timeout: time.Second * 10,
	}
	resp, err := client.Do(req)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: commit request failed")
	}

	if resp.StatusCode != 201 {
		return nil, fmt.Errorf("rooset: commit request failed with status %d", resp.StatusCode)
	}

	var parsedBody createCommitGitlabResp
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: failed to parse response body")
	}

	if err := json.Unmarshal(body, &parsedBody); err != nil {
		return nil, errors.Wrap(err, "rooset: failed to parse response body")
	}

	return &GitRecord{BranchName: branch, SHA: parsedBody.ID}, nil
}
