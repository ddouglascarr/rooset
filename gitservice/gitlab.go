package gitservice

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/pkg/errors"
)

//SHA is the sha of a git commit
type SHA string

type createCommitGitlabResp struct {
	ID string `json:"id"`
}

type gitlabCommitsReq struct {
	ID            string   `json:"id"`
	Branch        string   `json:"branch"`
	CommitMessage string   `json:"commit_message"`
	StartBranch   string   `json:"start_branch"`
	Actions       []Action `json:"actions"`
}

func createCommit(
	repositoryName string,
	startBranch string,
	branch string,
	actions []Action,
) (*GitRecord, error) {
	gitlabID := fmt.Sprintf("%s%%2F%s", conf.Gitlab.AccountName, repositoryName)
	reqBody, err := json.Marshal(gitlabCommitsReq{
		ID:            repositoryName,
		Branch:        branch,
		CommitMessage: "createCommit",
		StartBranch:   startBranch,
		Actions:       actions,
	})
	if err != nil {
		return nil, errors.Wrap(err, "rooset: gitlab request marshall failed")
	}
	url := fmt.Sprintf(
		"%s/api/v4/projects/%s/repository/commits",
		conf.Gitlab.Host, gitlabID)
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(reqBody))
	if err != nil {
		return nil, errors.Wrap(err, "rooset: commit request failed")
	}
	req.Header.Add("Private-Token", conf.Gitlab.BearerTk)
	req.Header.Add("Accept", "application/json")
	req.Header.Add("Content-Type", "application/json")

	client := &http.Client{
		Timeout: time.Second * 10,
	}
	resp, err := client.Do(req)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: commit request failed")
	}
	defer resp.Body.Close()

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
