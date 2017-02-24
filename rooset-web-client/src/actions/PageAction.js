// @flow
import type { uuid } from "../types";

export type DISPLAY_USER_PAGE_ACTION = "DISPLAY_USER_PAGE_ACTION";
export type DISPLAY_UNIT_PAGE_ACTION = "DISPLAY_UNIT_PAGE_ACTION";

type DisplayUnitPagePayload = {| id: uuid |};
export class DisplayUnitPageAction {
  type: DISPLAY_USER_PAGE_ACTION;
  payload: DisplayUnitPagePayload;

  constructor(payload: DisplayUnitPagePayload) {
    this.payload = payload;
  }
}

export class DisplayUserPageAction {
  type: DISPLAY_UNIT_PAGE_ACTION;
  payload: {| id: uuid |};

  constructor(id: uuid) {
    this.payload = { id };
  }
}

export type PageAction = DisplayUnitPageAction | DisplayUserPageAction;
