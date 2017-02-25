// @flow

import type { UUID } from "uuid-js";

export type int = number;
export type uint = number;
export type ullong = number;

export type uuid = UUID<4>;

export type area = {
  areaId: uuid,
  name: string,
  description: string,
  externalReference: string,
};

export type date = number;

export type duration = number;

export type keyNamePair = {
  key: uuid,
  name: string,
};

export type initiative = {
  id: uuid,
  initiativeId: uuid,
  requesterid: uuid,
  name: string,
  content: string,
  created: date,
};

export type member = {
  id: uuid,
  memberId: uuid,
  pollingRight: boolean,
  votingRight: boolean,
  initiativeRight: boolean,
  managementRight: boolean,
  weight: number,
};

export type uuidNullable = uuid | null;

export type schulzeBallot = {
  approve: Array<Array<uuid>>,
  abstain: Array<uuid>,
  disapprove: Array<Array<uuid>>,
};

export type email = string;

export type exceptionCode =
  | "CONFLICT_EXCEPTION"
  | "GENERAL_PROJECT_EXCEPTION"
  | "ITEM_NOT_FOUND_EXCEPTION"
  | "UNAUTHORIZED_EXCEPTION"
  | "UNPRIVILEGED_EXCEPTION"
  | "INVALID_INPUT_EXCEPTION"
  | "ISSUE_STATE_EXCEPTION";

export type issueState =
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

export type exceptionType =
  | "COMMAND_EVALUATION_EXCEPTION"
  | "QUERY_EXCEPTION"
  | "SYSTEM_EXCEPTION";
