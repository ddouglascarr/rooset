package lfclient

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	"github.com/pkg/errors"
)

// IssueState enum
type IssueState string

const (
	Idle         = IssueState("idle")
	Admission    = IssueState("admission")
	Discussion   = IssueState("discussion")
	Verification = IssueState("verification")
	Voting       = IssueState("voting")
)

func (s IssueState) IsValid() bool {
	switch s {
	case Idle:
	case Admission:
	case Discussion:
	case Verification:
	case Voting:
	default:
		return false
	}
	return true
}

// Initiative represents a lf initiative
type Initiative struct {
	ID          int        `json:"id"`
	IssueID     int        `json:"issue_id"`
	Name        string     `json:"name"`
	InitiatorID int        `json:"initator_id"`
	IssueState  IssueState `json:"issue_state"`
}

// IsActive determines if the intiiative is still live
func (i *Initiative) IsActive() bool {
	// TODO: implement
	return true
}

var client = &http.Client{
	Timeout: 10 * time.Second,
}

// GetInitiative gets a lq initiative
func GetInitiative(initiativeID int) (*Initiative, error) {
	url := fmt.Sprintf("http://localhost:8080/initiative/show/%d.html", initiativeID)
	log.Println(url)
	var parsedResp Initiative
	req, err := http.NewRequest(
		"GET",
		url,
		nil,
	)
	req.Header.Set("Accept", "application/json")
	// TODO: lol
	req.Header.Set("Cookie", "liquid_feedback_session=IehzbSC5WwGQRjiPRE1y64gZ1F4AI8X8;")
	if err != nil {
		return nil, errors.Wrap(err, "failed to fetch initiative")
	}

	resp, err := client.Do(req)
	if err != nil {
		return nil, errors.Wrap(err, "failed to fetch initiative")
	}

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("failed to fetch initiative, status code: %d", resp.StatusCode)
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, errors.Wrap(err, "failed to fetch initiative")
	}
	err = json.Unmarshal(body, &parsedResp)
	if err != nil {
		return nil, errors.Wrap(err, "failed to parse initiative")
	}
	return &parsedResp, err
}
