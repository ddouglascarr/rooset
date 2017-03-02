// @flow

import type { UUID } from "uuid-js";

export type Int = number;
export type Uint = number;
export type Ullong = number;

export type Uuid = UUID<4>;

export type Area = {|
  areaId: Uuid,
  name: string,
  description: string,
  externalReference: string,
|};

export type Date = number;

export type Duration = number;

export type KeyNamePair = {|
  key: Uuid,
  name: string,
|};

export type Initiative = {|
  id: Uuid,
  initiativeId: Uuid,
  requesterid: Uuid,
  name: string,
  content: string,
  created: Date,
|};

export type Member = {|
  id: Uuid,
  memberId: Uuid,
  pollingRight: boolean,
  votingRight: boolean,
  initiativeRight: boolean,
  managementRight: boolean,
  weight: number,
|};

export type SchulzeBallot = {|
  approve: Array<Array<Uuid>>,
  abstain: Array<Uuid>,
  disapprove: Array<Array<Uuid>>,
|};

export type Email = string;

export type ExceptionCode =
  | "CONFLICT_EXCEPTION"
  | "GENERAL_PROJECT_EXCEPTION"
  | "ITEM_NOT_FOUND_EXCEPTION"
  | "UNAUTHORIZED_EXCEPTION"
  | "UNPRIVILEGED_EXCEPTION"
  | "INVALID_INPUT_EXCEPTION"
  | "ISSUE_STATE_EXCEPTION";

export type IssueState =
  | "ADMISSION"
  | "DISCUSSION"
  | "VERIFICATION"
  | "VOTING"
  | "CANCELED_BY_ADMIN"
  | "CANCELED_REVOKED_BEFORE_ACCEPTED"
  | "CANCELED_ISSUE_NOT_ACCEPTED"
  | "CANCELED_AFTER_REVOCATION_DURING_DISCUSSION"
  | "CANCELED_AFTER_REVOCATION_DURING_VERIFICATION"
  | "CANCELED_NO_INITIATIVE_ADMITTED"
  | "FINISHED_TABULATING_RESULTS"
  | "FINISHED_WITHOUT_WINNER"
  | "FINISHED_WITH_WINNER";

export type ExceptionType =
  | "COMMAND_EVALUATION_EXCEPTION"
  | "QUERY_EXCEPTION"
  | "SYSTEM_EXCEPTION";

export type Policy = {|
  policyId: Uuid,
  name: string,
  description: string,
  discussionTime: Duration,
  verificationTime: Duration,
  votingTime: Duration,
  issueQuorumNum: Uint,
  issueQuorumDen: Uint,
|};

export type ExceptionPayload = {|
  code: ExceptionType,
  message: string,
|};
