package messages

//NewInitiativeReq request body for /new-initiative
type NewInitiativeReq struct {
	FileActions []FileAction
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
