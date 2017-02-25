
// @flow
import type {
  Uuid,
  Date,
  Area,
  ExceptionPayload,
  Initiative,
  IssueState,
  KeyNamePair,
  Member,
  Ullong,
  Policy,
} from '../../types';
import { template } from "lodash";



export type ISSUE_QUERY_RESPONSE = "ISSUE_QUERY_RESPONSE";

type IssueQueryResponsePayload = {|
  id: Uuid,
  unitId: Uuid,
  areaId: Uuid,
  policyId: Uuid,
  created: Date,
  issueState: IssueState,
  resolved: boolean,
  initiatives: Array<KeyNamePair>
|};

export class IssueQueryResponse {
  type: ISSUE_QUERY_RESPONSE;
  payload: IssueQueryResponsePayload;

  constructor(payload: IssueQueryResponsePayload) {
    this.payload = payload;
    this.type = "ISSUE_QUERY_RESPONSE";
  }
}



export type UNIT_MEMBER_QUERY_RESPONSE = "UNIT_MEMBER_QUERY_RESPONSE";

type UnitMemberQueryResponsePayload = {|
  id: Uuid,
  members: Array<Member>,
  totalWeight: Ullong
|};

export class UnitMemberQueryResponse {
  type: UNIT_MEMBER_QUERY_RESPONSE;
  payload: UnitMemberQueryResponsePayload;

  constructor(payload: UnitMemberQueryResponsePayload) {
    this.payload = payload;
    this.type = "UNIT_MEMBER_QUERY_RESPONSE";
  }
}



export type UNIT_QUERY_RESPONSE = "UNIT_QUERY_RESPONSE";

type UnitQueryResponsePayload = {|
  id: Uuid,
  name: string,
  description: string,
  policy: Policy,
  areas: Array<Area>
|};

export class UnitQueryResponse {
  type: UNIT_QUERY_RESPONSE;
  payload: UnitQueryResponsePayload;

  constructor(payload: UnitQueryResponsePayload) {
    this.payload = payload;
    this.type = "UNIT_QUERY_RESPONSE";
  }
}



export type ISSUE_QUERY_REQUEST = "ISSUE_QUERY_REQUEST";

type IssueQueryRequestPayload = {|
  id: Uuid
|};

export class IssueQueryRequest {
  type: ISSUE_QUERY_REQUEST;
  payload: IssueQueryRequestPayload;
  isHttpQueryAction: true;

  constructor(payload: IssueQueryRequestPayload) {
    this.payload = payload;
    this.type = "ISSUE_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }

  getHttpUri(): string {
    return applyUriTemplate("/issues/{id}", this.payload);
  }

  buildResponse(payload: IssueQueryResponsePayload)
    : IssueQueryResponse
  {
    return new IssueQueryResponse(payload);
  }
}


export type UNIT_MEMBER_QUERY_REQUEST = "UNIT_MEMBER_QUERY_REQUEST";

type UnitMemberQueryRequestPayload = {|
  id: Uuid
|};

export class UnitMemberQueryRequest {
  type: UNIT_MEMBER_QUERY_REQUEST;
  payload: UnitMemberQueryRequestPayload;
  isHttpQueryAction: true;

  constructor(payload: UnitMemberQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_MEMBER_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }

  getHttpUri(): string {
    return applyUriTemplate("/units/{id}/members", this.payload);
  }

  buildResponse(payload: UnitMemberQueryResponsePayload)
    : UnitMemberQueryResponse
  {
    return new UnitMemberQueryResponse(payload);
  }
}


export type UNIT_QUERY_REQUEST = "UNIT_QUERY_REQUEST";

type UnitQueryRequestPayload = {|
  id: Uuid
|};

export class UnitQueryRequest {
  type: UNIT_QUERY_REQUEST;
  payload: UnitQueryRequestPayload;
  isHttpQueryAction: true;

  constructor(payload: UnitQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }

  getHttpUri(): string {
    return applyUriTemplate("/units/{id}", this.payload);
  }

  buildResponse(payload: UnitQueryResponsePayload)
    : UnitQueryResponse
  {
    return new UnitQueryResponse(payload);
  }
}


export type ISSUE_QUERY_ERROR = "ISSUE_QUERY_ERROR";

export class IssueQueryError {
  type: ISSUE_QUERY_ERROR;
  payload: ExceptionPayload;
  error: true;

  constructor(payload: ExceptionPayload) {
    this.payload = payload;
    this.error = true;
  }
}


export type UNIT_MEMBER_QUERY_ERROR = "UNIT_MEMBER_QUERY_ERROR";

export class UnitMemberQueryError {
  type: UNIT_MEMBER_QUERY_ERROR;
  payload: ExceptionPayload;
  error: true;

  constructor(payload: ExceptionPayload) {
    this.payload = payload;
    this.error = true;
  }
}


export type UNIT_QUERY_ERROR = "UNIT_QUERY_ERROR";

export class UnitQueryError {
  type: UNIT_QUERY_ERROR;
  payload: ExceptionPayload;
  error: true;

  constructor(payload: ExceptionPayload) {
    this.payload = payload;
    this.error = true;
  }
}


function applyUriTemplate(uriTmpl, vars) {
  const opts = { interpolate:  /{(.+?)}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
