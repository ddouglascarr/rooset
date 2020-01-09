package messages

//FileActionType enum for creating actions
type FileActionType string

const (
	//Create action creates a new file
	Create = FileActionType("create")
	//Update action updates an already existing file
	Update = FileActionType("update")
)

//FileAction describes the creation or deletion of a single file
type FileAction struct {
	Action   FileActionType
	FileName string
	Content  string
}

//SHA is the sha of a git commit
type SHA string

//GitRecord given to lffrontend for foreign key reference.
type GitRecord struct {
	BranchName string
	SHA        string
}

//BlobRecord describes a blob in the repo
type BlobRecord struct {
	SHA  string
	Name string
}

//Blob is a blob in the repo
type Blob struct {
	SHA     string
	Content string
}
