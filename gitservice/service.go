package gitservice

import (
	"github.com/ddouglascarr/rooset/gitlab"
	"github.com/ddouglascarr/rooset/messages"
)

func listAreaFiles(repositoryName string, areaID int64, SHA string) ([]*messages.BlobRecord, error) {
	return gitlab.ListGitlabAreaBlobs(repositoryName, areaID, SHA)
}

func getBlob(repositoryName, SHA string) (*messages.Blob, error) {
	return gitlab.GetGitlabBlob(repositoryName, SHA)
}

func createInitiative(
	repositoryName string,
	areaID int64,
	content string,
) (*messages.CommitRecord, error) {
	return gitlab.CreateGitlabCommit(
		repositoryName,
		areaID,
		"master",
		genID(),
		[]*messages.FileAction{
			&messages.FileAction{
				Action:   messages.FileActionType_Create,
				FileName: "doc.json",
				Content:  content,
			},
		},
	)
}

func updateInitiative(
	repositoryName string,
	actions []messages.FileAction,
	prevRecord *messages.CommitRecord,
) (*messages.CommitRecord, error) {
	return nil, nil
}

func createSuggestion(
	repositoryName string,
	actions []messages.FileAction,
	initiativeRecord *messages.CommitRecord,
) (*messages.CommitRecord, error) {
	return nil, nil
}

func updateSuggestion(
	repositoryName string,
	actions []messages.FileAction,
	prevRecord *messages.CommitRecord,
) (*messages.CommitRecord, error) {
	return nil, nil
}
