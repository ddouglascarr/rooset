package messages

type SHA = string
type JWTStr = string
type UserID = int64

//
// CreateDoc
//

type CreateDocReqClaims struct {
	XOp     []string `json:"Op"`
	UserID  UserID   // TODO: use this field for rate limiting
	BaseSHA SHA
}

func (c *CreateDocReqClaims) Valid() error { return nil }
func (c CreateDocReqClaims) Op() []string  { return c.XOp }

type CreateDocReqBody struct {
	Content string
}

type CreateDocReq struct {
	UserID  UserID
	BaseSHA SHA
	Content string
}

type CreateDocResp struct {
	SHA                SHA
	BaseSHA            SHA
	ModifiedSectionIDs []string
	Tk                 JWTStr
}

//
// GetDoc
//

type GetDocReqClaims struct {
	XOp    []string `json:"Op"`
	UserID UserID   // TODO: use for rate limiting
	SHA    SHA
}

func (c GetDocReqClaims) Valid() error { return nil }
func (c GetDocReqClaims) Op() []string { return c.XOp }

type GetDocResp struct {
	SHA     SHA
	Content string
}
