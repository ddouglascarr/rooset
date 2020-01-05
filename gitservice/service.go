package gitservice

import (
	"fmt"
	"strings"
)

//ActionType enum for creating actions
type ActionType string

const (
	create = ActionType("create")
	update = ActionType("update")
)

//Action describes the creation or deletion of a single file
type Action struct {
	Action   ActionType `json:"action"`
	FilePath string     `json:"file_path"`
	Content  string     `json:"content"`
}

//GitRecord given to lffrontend for foreign key reference.
type GitRecord struct {
	BranchName string
	SHA        string
}

func createInitiative(
	repositoryName string,
	areaID int64,
	actions []Action,
) (*GitRecord, error) {
	err := validateActionPaths(areaID, actions)
	if err != nil {
		return nil, err
	}
	return createCommit(repositoryName, "master", genID(), actions)
}

func updateInitiative(
	repositoryName string,
	actions []Action,
	prevRecord *GitRecord,
) (*GitRecord, error) {
	return nil, nil
}

func createSuggestion(
	repositoryName string,
	actions []Action,
	initiativeRecord *GitRecord,
) (*GitRecord, error) {
	return nil, nil
}

func updateSuggestion(
	repositoryName string,
	actions []Action,
	prevRecord *GitRecord,
) (*GitRecord, error) {
	return nil, nil
}

func validateActionPaths(areaID int64, actions []Action) error {
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
