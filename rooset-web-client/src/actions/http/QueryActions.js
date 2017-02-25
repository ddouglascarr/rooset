
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

type issueQueryResponsePayload = {|
  id: Uuid,
  unitId: Uuid,
  areaId: Uuid,
  policyId: Uuid,
  created: Date,
  issueState: IssueState,
  resolved: boolean,
  initiatives: Array<KeyNamePair>
|};

export class issueQueryResponse {
  type: ISSUE_QUERY_RESPONSE;
  payload: issueQueryResponsePayload;

  constructor(payload: issueQueryResponsePayload) {
    this.payload = payload;
    this.type = "ISSUE_QUERY_RESPONSE";
  }
}



export type UNIT_MEMBER_QUERY_RESPONSE = "UNIT_MEMBER_QUERY_RESPONSE";

type unitMemberQueryResponsePayload = {|
  id: Uuid,
  members: Array<Member>,
  totalWeight: Ullong
|};

export class unitMemberQueryResponse {
  type: UNIT_MEMBER_QUERY_RESPONSE;
  payload: unitMemberQueryResponsePayload;

  constructor(payload: unitMemberQueryResponsePayload) {
    this.payload = payload;
    this.type = "UNIT_MEMBER_QUERY_RESPONSE";
  }
}



export type UNIT_QUERY_RESPONSE = "UNIT_QUERY_RESPONSE";

type unitQueryResponsePayload = {|
  id: Uuid,
  name: string,
  description: string,
  policy: Policy,
  areas: Array<Area>
|};

export class unitQueryResponse {
  type: UNIT_QUERY_RESPONSE;
  payload: unitQueryResponsePayload;

  constructor(payload: unitQueryResponsePayload) {
    this.payload = payload;
    this.type = "UNIT_QUERY_RESPONSE";
  }
}



export type ISSUE_QUERY_REQUEST = "ISSUE_QUERY_REQUEST";

type issueQueryRequestPayload = {|
  id: Uuid
|};

export class issueQueryRequest {
  type: ISSUE_QUERY_REQUEST;
  payload: issueQueryRequestPayload;
  isHttpQueryAction: true;
  constructor(payload: issueQueryRequestPayload) {
    this.payload = payload;
    this.type = "ISSUE_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }
  getHttpUri(): string {
    return applyUriTemplate("/issues/{id}", this.payload);
  }
}


export type UNIT_MEMBER_QUERY_REQUEST = "UNIT_MEMBER_QUERY_REQUEST";

type unitMemberQueryRequestPayload = {|
  id: Uuid
|};

export class unitMemberQueryRequest {
  type: UNIT_MEMBER_QUERY_REQUEST;
  payload: unitMemberQueryRequestPayload;
  isHttpQueryAction: true;
  constructor(payload: unitMemberQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_MEMBER_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }
  getHttpUri(): string {
    return applyUriTemplate("/units/{id}/members", this.payload);
  }
}


export type UNIT_QUERY_REQUEST = "UNIT_QUERY_REQUEST";

type unitQueryRequestPayload = {|
  id: Uuid
|};

export class unitQueryRequest {
  type: UNIT_QUERY_REQUEST;
  payload: unitQueryRequestPayload;
  isHttpQueryAction: true;
  constructor(payload: unitQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_QUERY_REQUEST";
    this.isHttpQueryAction = true;
  }
  getHttpUri(): string {
    return applyUriTemplate("/units/{id}", this.payload);
  }
}


function applyUriTemplate(uriTmpl, vars) {
  const opts = { interpolate:  /{(.+?)}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
