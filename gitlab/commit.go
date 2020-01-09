package gitlab

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/pkg/errors"
)

type createCommitGitlabResp struct {
	ID string `json:"id"`
}

type gitlabAction struct {
	Action   messages.ActionType `json:"action"`
	FilePath string              `json:"file_path"`
	Content  string              `json:"content"`
}

func convertActions(actions []messages.Action) []gitlabAction {
	gitlabActions := make([]gitlabAction, len(actions))
	for idx, action := range actions {
		gitlabActions[idx] = gitlabAction(action)
	}
	return gitlabActions
}

type gitlabCommitsReq struct {
	ID            string         `json:"id"`
	Branch        string         `json:"branch"`
	CommitMessage string         `json:"commit_message"`
	StartBranch   string         `json:"start_branch"`
	Actions       []gitlabAction `json:"actions"`
}

//CreateGitlabCommit create a commit with a collection of actions
func CreateGitlabCommit(
	repositoryName string,
	startBranch string,
	branch string,
	actions []messages.Action,
) (*messages.GitRecord, error) {
	reqBody, err := json.Marshal(gitlabCommitsReq{
		ID:            repositoryName,
		Branch:        branch,
		CommitMessage: "createCommit",
		StartBranch:   startBranch,
		Actions:       convertActions(actions),
	})
	if err != nil {
		return nil, errors.Wrap(err, "rooset: gitlab request marshall failed")
	}
	url := fmt.Sprintf(
		"%s/api/v4/projects/%s/repository/commits",
		conf.Gitlab.Host, buildGitlabID(repositoryName))
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

	return &messages.GitRecord{BranchName: branch, SHA: parsedBody.ID}, nil
}
