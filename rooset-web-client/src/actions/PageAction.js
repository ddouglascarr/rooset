// @flow
import type { Uuid } from "../types";

export type DISPLAY_USER_PAGE_ACTION = "DISPLAY_USER_PAGE_ACTION";
export type DISPLAY_UNIT_PAGE_ACTION = "DISPLAY_UNIT_PAGE_ACTION";

type DisplayUnitPagePayload = {| id: Uuid |};
export class DisplayUnitPageAction {
  type: DISPLAY_USER_PAGE_ACTION;
  payload: DisplayUnitPagePayload;

  constructor(payload: DisplayUnitPagePayload) {
    this.payload = payload;
  }
}

export class DisplayUserPageAction {
  type: DISPLAY_UNIT_PAGE_ACTION;
  payload: {| id: Uuid |};

  constructor(id: Uuid) {
    this.payload = { id };
  }
}

export type PageAction = DisplayUnitPageAction | DisplayUserPageAction;
