package github

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"

	"github.com/pkg/errors"
)

// const openPRsQuery = `
//   query OpenPullRequests($RepositoryID: ID!) {
// 	repo: node(id: $RepositoryID) {
// 	  ...repoFields
// 	}
//   }
//
//   fragment repoFields on Repository {
// 	id
// 	  name
// 	  pullRequests(first: 100, states: OPEN) {
// 		edges {
// 		  node {
// 			...prFields
// 		  }
// 		}
// 	  }
//   }
//
//   fragment prFields on PullRequest {
// 	state
// 	title
// 	baseRefName
// 	createdAt
// 	id
// 	mergeable
// 	author {
// 	  login
// 	}
// 	headRef {
// 	  name
// 	  target {
// 		oid
//
// 		repository {
// 		  nameWithOwner
// 		  id
// 		}
// 	  }
// 	}
//   }
// `

type graphQLQueryBody struct {
	OperationName string      `json:"operationName"`
	Query         string      `json:"query"`
	Variables     interface{} `json:"variables"`
}

// NewGraphQLQueryBody builds a JSON query string for a graphql query
func NewGraphQLQueryBody(
	operationName string,
	query string,
	variables interface{},
) ([]byte, error) {
	body, err := json.Marshal(graphQLQueryBody{
		OperationName: operationName,
		Query:         query,
		Variables:     variables,
	})
	if err != nil {
		return body, errors.Wrap(err, "rooset: invalid query")
	}

	return body, nil
}

// FetchRepoID fetches the GraphQL node_id for a repo
// this is necessary because the Github installation webhook doesn't include
// it for the repos that have installations.
func FetchRepoID(installationID int64, owner, name string) (string, error) {
	tk, err := FetchInstallationTk(installationID)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to fetch repo id")
	}
	body, err := NewGraphQLQueryBody(
		"FetchRepoIDQuery",
		` 
		query FetchRepoIDQuery($Owner:String!, $Name:String!) {
			repo: repository(owner: $Owner, name: $Name) {
				id
			},
		}
		`,
		struct {
			Owner string
			Name  string
		}{owner, name},
	)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to fetch repo id")
	}
	client := &http.Client{
		Timeout: time.Second * 10,
	}
	req, err := http.NewRequest(
		"POST",
		"https://api.github.com/graphql",
		bytes.NewBuffer(body),
	)
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to GET app endpoint")
	}
	req.Header.Add("Authorization", fmt.Sprintf("token %s", tk))
	req.Header.Add("Accept", "application/vnd.github.machine-man-preview+json")
	// req.Header.Add("Content-Type", "application/json; charset=utf-8")

	resp, err := client.Do(req)
	if err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}

	if resp.StatusCode != 200 {
		return "", errors.New(fmt.Sprintf("rooset: FetchRepoID query failed %s", resp.Status))
	}

	respBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}

	parsedBody := struct {
		Data struct {
			Repo struct {
				ID string `json:"id"`
			} `json:"repo"`
		} `json:"data"`
		Errors []struct {
			Message string `json:"message"`
		} `json:"errors"`
	}{}
	if err := json.Unmarshal(respBody, &parsedBody); err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}
	if len(parsedBody.Errors) > 0 {
		return "", errors.New(fmt.Sprintf(
			"rooset: FetchRepoID failed - %s", parsedBody.Errors[0].Message),
		)
	}

	return parsedBody.Data.Repo.ID, nil
}
