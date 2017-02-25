
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


export type ISSUE_QUERY_REQUEST = "ISSUE_QUERY_REQUEST";

type issueQueryRequestPayload = {|
  id: Uuid
|};

export class issueQueryRequest {
  type: ISSUE_QUERY_REQUEST;
  payload: issueQueryRequestPayload;
  constructor(payload: issueQueryRequestPayload) {
    this.payload = payload;
    this.type = "ISSUE_QUERY_REQUEST";
  }
}


export type UNIT_MEMBER_QUERY_REQUEST = "UNIT_MEMBER_QUERY_REQUEST";

type unitMemberQueryRequestPayload = {|
  id: Uuid
|};

export class unitMemberQueryRequest {
  type: UNIT_MEMBER_QUERY_REQUEST;
  payload: unitMemberQueryRequestPayload;
  constructor(payload: unitMemberQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_MEMBER_QUERY_REQUEST";
  }
}


export type UNIT_QUERY_REQUEST = "UNIT_QUERY_REQUEST";

type unitQueryRequestPayload = {|
  id: Uuid
|};

export class unitQueryRequest {
  type: UNIT_QUERY_REQUEST;
  payload: unitQueryRequestPayload;
  constructor(payload: unitQueryRequestPayload) {
    this.payload = payload;
    this.type = "UNIT_QUERY_REQUEST";
  }
}



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


