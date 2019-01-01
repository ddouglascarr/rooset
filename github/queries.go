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

const fetchRepoIDQuery = `
  query FetchRepoIDQuery($repoOwner:String!, $repoName:String!) {
	repo: repository(owner: $repoOwner, name: $repoName) {
	  id
	},
  }
`

type graphQLQueryBody struct {
	operationName string
	query         string
	variables     interface{}
}

// NewGraphQLQueryBody builds a JSON query string for a graphql query
func NewGraphQLQueryBody(
	operationName string,
	query string,
	variables interface{},
) ([]byte, error) {
	body, err := json.Marshal(graphQLQueryBody{
		operationName: operationName,
		query:         query,
		variables:     variables,
	})
	if err != nil {
		return body, errors.Wrap(err, "rooset: invalid query")
	}

	return body, nil
}

type fetchRepoIDVars struct {
	owner string
	name  string
}

// FetchRepoID fetches the GraphQL node_id for a repo
// this is necessary because the Github installation webhook doesn't include
// it for the repos that have installations.
func FetchRepoID(owner, name string) (string, error) {
	tk, err := NewGithubAppJWT()
	if err != nil {
		return "", errors.Wrap(err, "rooset: failed to fetch repo id")
	}
	body, err := NewGraphQLQueryBody(
		"FetchRepoIDQuery",
		fetchRepoIDQuery,
		fetchRepoIDVars{owner: owner, name: name},
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
	req.Header.Add("Authorization", fmt.Sprintf("Bearer %s", tk))
	req.Header.Add("Accept", "application/vnd.github.machine-man-preview+json")
	req.Header.Add("Content-Type", "application/json; charset=utf-8")

	resp, err := client.Do(req)
	if err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}

	respBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}

	parsedBody := struct {
		Data struct {
			Repo struct {
				ID string
			}
		}
	}{}
	if err := json.Unmarshal(respBody, &parsedBody); err != nil {
		return "", errors.Wrap(err, "rooset: FetchRepoID query failed")
	}

	return parsedBody.Data.Repo.ID, nil
}
