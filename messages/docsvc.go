package messages

type SHA = string
type JWTStr = string
type UserID = int64

//
// CreateRev
//

type CreateRevReqClaims struct {
	XOp     []string `json:"Op"`
	UserID  UserID   // TODO: use this field for rate limiting
	BaseSHA SHA
}

// TODO: validate not empty
func (c *CreateRevReqClaims) Valid() error { return nil }
func (c CreateRevReqClaims) Op() []string  { return c.XOp }

// TODO: Valid
type CreateRevReqBody struct {
	Content string
}

type CreateRevReq struct {
	UserID  UserID
	BaseSHA SHA
	Content string
}

type CreateRevResp struct {
	SHA                SHA
	BaseSHA            SHA
	ModifiedSectionIDs []string
	Tk                 JWTStr
}

//
// Get
//

type GetReqClaims struct {
	XOp    []string `json:"Op"`
	UserID UserID   // TODO: use for rate limiting
}

// TODO validate not empty
func (c GetReqClaims) Valid() error { return nil }
func (c GetReqClaims) Op() []string { return c.XOp }

// TODO validate not empty
type GetReqBody struct {
	SHA SHA
}

type GetResp struct {
	SHA     SHA
	Content string
}

//
// CreateDesc
//

type CreateDescReqClaims struct {
	XOp    []string `json:"Op"`
	UserID UserID   // TODO: use for rate limiting
}

// TODO validate not empty
func (c CreateDescReqClaims) Valid() error { return nil }
func (c CreateDescReqClaims) Op() []string { return c.XOp }

type CreateDescReqBody struct {
	Content string
}

type CreateDescReq struct {
	UserID  UserID // TODO: use for rate limiting
	Content string
}

type CreateDescResp struct {
	Tk JWTStr
}
