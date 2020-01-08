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

type gitlabAction struct {
	Action   ActionType `json:"action"`
	FilePath string     `json:"file_path"`
	Content  string     `json:"content"`
}

func convertActions(actions []Action) []gitlabAction {
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

func createGitlabCommit(
	repositoryName string,
	startBranch string,
	branch string,
	actions []Action,
) (*GitRecord, error) {
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

	return &GitRecord{BranchName: branch, SHA: parsedBody.ID}, nil
}

func buildGitlabID(repositoryName string) string {
	return fmt.Sprintf("%s%%2F%s", conf.Gitlab.AccountName, repositoryName)
}

type gitlabTreeResp struct {
	SHA  string `json:"id"`
	Name string `json:"name"`
	Type string `json:"type"`
}

func convertTreeResps(resps []gitlabTreeResp) []BlobRecord {
	var blobRecords []BlobRecord
	for _, resp := range resps {
		if resp.Type == "blob" {
			blobRecords = append(blobRecords, BlobRecord{
				SHA:  resp.SHA,
				Name: resp.Name,
			})
		}
	}
	return blobRecords
}

func listGitlabAreaBlobs(repositoryName string, areaID int64, SHA string) ([]BlobRecord, error) {
	url := fmt.Sprintf(
		"%s/api/v4/projects/%s/repository/tree?path=%s&ref=%s",
		conf.Gitlab.Host,
		buildGitlabID(repositoryName),
		fmt.Sprintf("areas%%2F%d", areaID),
		SHA,
	)
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: listGitlabAreaBlobs request failed")
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

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("rooset: commit request failed with status %d", resp.StatusCode)
	}

	var parsedBody []gitlabTreeResp

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: failed to parse response body")
	}

	if err := json.Unmarshal(body, &parsedBody); err != nil {
		return nil, errors.Wrap(err, "rooset: failed to parse response body")
	}

	return convertTreeResps(parsedBody), nil
}

func getGitlabBlob(repositoryName string, SHA string) (*Blob, error) {
	url := fmt.Sprintf(
		"%s/api/v4/projects/%s/repository/blobs/%s/raw",
		conf.Gitlab.Host,
		buildGitlabID(repositoryName),
		SHA,
	)
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: listGitlabAreaBlobs request failed")
	}
	req.Header.Add("Private-Token", conf.Gitlab.BearerTk)
	req.Header.Add("Accept", "text/plain")

	client := &http.Client{
		Timeout: time.Second * 10,
	}
	resp, err := client.Do(req)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: commit request failed")
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("rooset: commit request failed with status %d", resp.StatusCode)
	}

	content, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: failed to parse response body")
	}

	return &Blob{SHA: SHA, Content: string(content)}, nil
}
