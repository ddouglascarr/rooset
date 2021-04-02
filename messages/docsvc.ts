export type SHA = string;
export type JWTStr = string;
export type UserID = string;

export type CreateRevReqClaims = {
    Op: string[];
    UserID: UserID;
    BaseSHA: SHA;
}
export type CreateRevReqBody = { Content: string }
export type CreateRevResp = {
    SHA: SHA;
    BaseSHA: SHA;
    ModifiedSectionIDs: string[];
    Tk: JWTStr;
}

export type GetReqBody = { SHA: string }
export type GetResp = {
    SHA: SHA;
    Content: string;
}

export type CreateDescReqClaims = {
    Op: string[];
    UserID: UserID;
}
export type CreateDescReqBody = { Content: string }
export type CreateDescResp = {
    Tk: JWTStr;
}

