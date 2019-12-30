package gitservice

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
	branch string,
	actions []Action, // will be validated externally TODO: mabye not?
) (*GitRecord, error) {
	return createCommit(repositoryName, "master", branch, actions)
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
