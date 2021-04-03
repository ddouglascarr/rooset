package messages

type SHA = string
type JWTStr = string
type UserID = int64

//
// CreateRev
//

type CreateRevReqClaims struct {
	XOp     []string `json:"ops"`
	UserID  UserID   `json:"user_id"` // TODO: use this field for rate limiting
	BaseSHA SHA      `json:"base_sha"`
}

// TODO: validate not empty
func (c *CreateRevReqClaims) Valid() error { return nil }
func (c CreateRevReqClaims) Op() []string  { return c.XOp }

// TODO: Valid
type CreateRevReqBody struct {
	Content string `json:"content"`
}

type CreateRevReq struct {
	UserID  UserID `json:"user_id"`
	BaseSHA SHA    `json:"base_sha"`
	Content string `json:"content"`
}

type CreateRevResp struct {
	SHA                SHA      `json:"sha"`
	BaseSHA            SHA      `json:"base_sha"`
	ModifiedSectionIDs []string `json:"modified_section_ids"`
	Tk                 JWTStr   `json:"tk"`
}

//
// Get
//

type GetReqClaims struct {
	XOp    []string `json:"ops"`
	UserID UserID   `json:"user_id"` // TODO: use for rate limiting
}

// TODO validate not empty
func (c GetReqClaims) Valid() error { return nil }
func (c GetReqClaims) Op() []string { return c.XOp }

// TODO validate not empty
type GetReqBody struct {
	SHA SHA `json:"sha"`
}

type GetResp struct {
	SHA     SHA    `json:"sha"`
	Content string `json:"content"`
}

//
// CreateDesc
//

type CreateDescReqClaims struct {
	XOp    []string `json:"ops"`
	UserID UserID   `json:"user_id"` // TODO: use for rate limiting
}

// TODO validate not empty
func (c CreateDescReqClaims) Valid() error { return nil }
func (c CreateDescReqClaims) Op() []string { return c.XOp }

type CreateDescReqBody struct {
	Content string `json:"content"`
}

type CreateDescReq struct {
	UserID  UserID `json:"user_id"` // TODO: use for rate limiting
	Content string `json:"content"`
}

type CreateDescResp struct {
	Tk JWTStr `json:"tk"`
}
