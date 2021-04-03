export type SHA = string;
export type JWTStr = string;
export type UserID = string;

export type CreateRevReqClaims = {
    ops: string[];
    user_id: UserID;
    base_sha: SHA;
}
export type CreateRevReqBody = { content: string }
export type CreateRevResp = {
    sha: SHA;
    base_sha: SHA;
    modified_section_ids: string[];
    tk: JWTStr;
}

export type GetReqBody = { sha: string }
export type GetResp = {
    sha: SHA;
    content: string;
}

export type CreateDescReqClaims = {
    ops: string[];
    user_id: UserID;
}
export type CreateDescReqBody = { content: string }
export type CreateDescResp = {
    tk: JWTStr;
}

