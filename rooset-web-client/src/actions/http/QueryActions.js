
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
import HttpRequest from "../../sys/HttpRequest";



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

export type IssueQueryResponse = {|
  type: ISSUE_QUERY_RESPONSE,
  payload: IssueQueryResponsePayload,
|}

export function buildIssueQueryResponse(payload: IssueQueryResponsePayload) :IssueQueryResponse {
  return {
    type: "ISSUE_QUERY_RESPONSE",
    payload: payload,
  }
}



export type UNIT_MEMBER_QUERY_RESPONSE = "UNIT_MEMBER_QUERY_RESPONSE";
type UnitMemberQueryResponsePayload = {|
  id: Uuid,
  members: Array<Member>,
  totalWeight: Ullong
|};

export type UnitMemberQueryResponse = {|
  type: UNIT_MEMBER_QUERY_RESPONSE,
  payload: UnitMemberQueryResponsePayload,
|}

export function buildUnitMemberQueryResponse(payload: UnitMemberQueryResponsePayload) :UnitMemberQueryResponse {
  return {
    type: "UNIT_MEMBER_QUERY_RESPONSE",
    payload: payload,
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

export type UnitQueryResponse = {|
  type: UNIT_QUERY_RESPONSE,
  payload: UnitQueryResponsePayload,
|}

export function buildUnitQueryResponse(payload: UnitQueryResponsePayload) :UnitQueryResponse {
  return {
    type: "UNIT_QUERY_RESPONSE",
    payload: payload,
  }
}



export type ISSUE_QUERY_ERROR = "ISSUE_QUERY_ERROR";

export type IssueQueryError = {
  type: ISSUE_QUERY_ERROR,
  error: true,
  payload: ExceptionPayload,
};

export function buildIssueQueryError(payload: ExceptionPayload) {
  return {
    type: "ISSUE_QUERY_ERROR",
    error:true,
    payload,
  };
}


export type UNIT_MEMBER_QUERY_ERROR = "UNIT_MEMBER_QUERY_ERROR";

export type UnitMemberQueryError = {
  type: UNIT_MEMBER_QUERY_ERROR,
  error: true,
  payload: ExceptionPayload,
};

export function buildUnitMemberQueryError(payload: ExceptionPayload) {
  return {
    type: "UNIT_MEMBER_QUERY_ERROR",
    error:true,
    payload,
  };
}


export type UNIT_QUERY_ERROR = "UNIT_QUERY_ERROR";

export type UnitQueryError = {
  type: UNIT_QUERY_ERROR,
  error: true,
  payload: ExceptionPayload,
};

export function buildUnitQueryError(payload: ExceptionPayload) {
  return {
    type: "UNIT_QUERY_ERROR",
    error:true,
    payload,
  };
}


export type ISSUE_QUERY_REQUEST = "ISSUE_QUERY_REQUEST";

type IssueQueryRequestPayload = {|
  id: Uuid
|};

export type IssueQueryRequest = {|
  type: ISSUE_QUERY_REQUEST,
  payload: IssueQueryRequestPayload,
|};

export function buildIssueQueryRequest(payload: IssueQueryRequestPayload) :IssueQueryRequest {
  return {
    type: "ISSUE_QUERY_REQUEST",
    payload,
  };
}


export type UNIT_MEMBER_QUERY_REQUEST = "UNIT_MEMBER_QUERY_REQUEST";

type UnitMemberQueryRequestPayload = {|
  id: Uuid
|};

export type UnitMemberQueryRequest = {|
  type: UNIT_MEMBER_QUERY_REQUEST,
  payload: UnitMemberQueryRequestPayload,
|};

export function buildUnitMemberQueryRequest(payload: UnitMemberQueryRequestPayload) :UnitMemberQueryRequest {
  return {
    type: "UNIT_MEMBER_QUERY_REQUEST",
    payload,
  };
}


export type UNIT_QUERY_REQUEST = "UNIT_QUERY_REQUEST";

type UnitQueryRequestPayload = {|
  id: Uuid
|};

export type UnitQueryRequest = {|
  type: UNIT_QUERY_REQUEST,
  payload: UnitQueryRequestPayload,
|};

export function buildUnitQueryRequest(payload: UnitQueryRequestPayload) :UnitQueryRequest {
  return {
    type: "UNIT_QUERY_REQUEST",
    payload,
  };
}


export type QueryRequest =
  | IssueQueryRequest
  | UnitMemberQueryRequest
  | UnitQueryRequest

export type QueryResponse =
  | IssueQueryResponse
  | UnitMemberQueryResponse
  | UnitQueryResponse

export type QueryError =
  | IssueQueryError
  | UnitMemberQueryError
  | UnitQueryError

export type QueryAction = QueryRequest | QueryResponse | QueryError;
