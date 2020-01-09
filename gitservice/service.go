package gitservice

import (
	"fmt"
	"strings"

	"github.com/ddouglascarr/rooset/gitlab"
	"github.com/ddouglascarr/rooset/messages"
)

func listAreaFiles(repositoryName string, areaID int64, SHA string) ([]messages.BlobRecord, error) {
	return gitlab.ListGitlabAreaBlobs(repositoryName, areaID, SHA)
}

func getBlob(repositoryName, SHA string) (*messages.Blob, error) {
	return gitlab.GetGitlabBlob(repositoryName, SHA)
}

func createInitiative(
	repositoryName string,
	areaID int64,
	actions []messages.Action,
) (*messages.GitRecord, error) {
	err := validateActionPaths(areaID, actions)
	if err != nil {
		return nil, err
	}
	return gitlab.CreateGitlabCommit(repositoryName, "master", genID(), actions)
}

func updateInitiative(
	repositoryName string,
	actions []messages.Action,
	prevRecord *messages.GitRecord,
) (*messages.GitRecord, error) {
	return nil, nil
}

func createSuggestion(
	repositoryName string,
	actions []messages.Action,
	initiativeRecord *messages.GitRecord,
) (*messages.GitRecord, error) {
	return nil, nil
}

func updateSuggestion(
	repositoryName string,
	actions []messages.Action,
	prevRecord *messages.GitRecord,
) (*messages.GitRecord, error) {
	return nil, nil
}

func validateActionPaths(areaID int64, actions []messages.Action) error {
	for _, action := range actions {
		for idx, piece := range strings.Split(action.FilePath, "/") {
			switch idx {
			case 0:
				if piece != "areas" {
					return fmt.Errorf("rooset: invalid FilePath %s", action.FilePath)
				}
			case 1:
				if string(areaID) == piece {
					return fmt.Errorf("rooset: unpermitted AreaID, should be %d, was %s", areaID, piece)
				}
			case 2:
				for i, p := range strings.Split(piece, ".") {
					switch i {
					case 0:
						continue
					case 1:
						if p != "json" {
							return fmt.Errorf("rooset: invalid FilePath %s, must end in .json", action.FilePath)
						}
					default:
						return fmt.Errorf("rooset: invalid FilePath %s, must end in .json", action.FilePath)
					}
				}
			default:
				return fmt.Errorf("rooset: invalid FilePath %s", action.FilePath)

			}
		}
	}
	return nil
}
