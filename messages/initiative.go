package messages

//ActionType enum for creating actions
type ActionType string

const (
	//Create action creates a new file
	Create = ActionType("create")
	//Update action updates an already existing file
	Update = ActionType("update")
)

//Action describes the creation or deletion of a single file
type Action struct {
	Action   ActionType
	FilePath string
	Content  string
}

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

//NewInitiativeReq request body for /new-initiative
type NewInitiativeReq struct {
	Actions []Action
}

//NewInitiativeResp response body for /new-initiative
type NewInitiativeResp struct {
	GitRecord GitRecord
}

//ListAreaBlobsReq request body for /list-area-blobs
type ListAreaBlobsReq struct {
	SHA string
}

//ListAreaBlobsResp response body for /list-area-blobs
type ListAreaBlobsResp struct {
	BlobRecords []BlobRecord
}

//GetBlobReq request body for /get-blob
type GetBlobReq struct {
	SHA string
}

//GetBlobResp response body for /get-blob
type GetBlobResp struct {
	Blob Blob
}
