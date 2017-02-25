
// @flow
import type {
  Uuid,
  Date,
  Area,
} from '../../types';


export type ISSUE_QUERY_REQUEST = "ISSUE_QUERY_REQUEST"

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


export type UNIT_MEMBER_QUERY_REQUEST = "UNIT_MEMBER_QUERY_REQUEST"

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


export type UNIT_QUERY_REQUEST = "UNIT_QUERY_REQUEST"

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


