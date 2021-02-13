export type SHA = string;
export type JWTStr = string;
export type UserID = string;

export type CreateDocReqClaims = {
    Op: string[];
    UserID: UserID;
    BaseSHA: SHA;
}
export type CreateDocReqBody = { Content: string }
export type CreateDocResp = {
    SHA: SHA;
    BaseSHA: SHA;
    ModifiedSectionIDs: string[];
    Tk: JWTStr;
}

export type GetDocResp = {
    SHA: SHA;
    Content: string;
}
