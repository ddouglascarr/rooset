package gitlab

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/pkg/errors"
)

//GetGitlabBlob fetches a raw blob by SHA
func GetGitlabBlob(repositoryName string, SHA string) (*messages.Blob, error) {
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

	return &messages.Blob{SHA: SHA, Content: string(content)}, nil
}

type gitlabTreeResp struct {
	SHA  string `json:"id"`
	Name string `json:"name"`
	Type string `json:"type"`
}

func convertTreeResps(resps []gitlabTreeResp) []messages.BlobRecord {
	var blobRecords []messages.BlobRecord
	for _, resp := range resps {
		if resp.Type == "blob" {
			blobRecords = append(blobRecords, messages.BlobRecord{
				SHA:  resp.SHA,
				Name: resp.Name,
			})
		}
	}
	return blobRecords
}

//ListGitlabAreaBlobs fetches all the blobs for an area at a commit
func ListGitlabAreaBlobs(repositoryName string, areaID int64, SHA string) ([]messages.BlobRecord, error) {
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

func buildGitlabID(repositoryName string) string {
	return fmt.Sprintf("%s%%2F%s", conf.Gitlab.AccountName, repositoryName)
}
