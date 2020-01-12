package gitlab

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/ddouglascarr/rooset/conf"
	"github.com/ddouglascarr/rooset/messages"
	"github.com/pkg/errors"
)

//GetDoc fetches a raw blob by SHA
func GetDoc(repositoryName string, areaID int64, gitRef string) (*messages.Blob, error) {
	url := fmt.Sprintf(
		"%s/api/v4/projects/%s/repository/files/areas%%2F%d%%2Fdoc.json/raw?ref=%s",
		conf.Gitlab.Host,
		buildGitlabID(repositoryName),
		areaID,
		gitRef,
	)
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: gitlab.GetDoc failed to build")
	}
	req.Header.Add("Private-Token", conf.Gitlab.BearerTk)
	req.Header.Add("Accept", "text/plain")

	client := &http.Client{
		Timeout: time.Second * 10,
	}
	resp, err := client.Do(req)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: gitlab.GetDoc request failed")
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("rooset: gitlab.GetDoc request failed with status %d", resp.StatusCode)
	}

	content, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, errors.Wrap(err, "rooset: gitlab.GetDoc failed to parse response body")
	}

	return &messages.Blob{SHA: gitRef, Content: string(content)}, nil
}
