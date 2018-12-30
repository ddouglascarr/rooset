package github

const openPRsQuery = `
  query OpenPullRequests($RepositoryID: ID!) {
	repo: node(id: $RepositoryID) {
	  ...repoFields
	}
  }

  fragment repoFields on Repository {
	id
	  name
	  pullRequests(first: 100, states: OPEN) {
		edges {
		  node {
			...prFields
		  }
		}
	  }
  }

  fragment prFields on PullRequest {
	state
	title
	baseRefName
	createdAt
	id
	mergeable
	author {
	  login
	}
	headRef {
	  name
	  target {
		oid
		
		repository {
		  nameWithOwner
		  id
		}
	  }
	}
  }
`
