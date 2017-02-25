// @flow
import type { Uuid } from "../types";

export type DISPLAY_USER_PAGE_ACTION = "DISPLAY_USER_PAGE_ACTION";
export type DISPLAY_UNIT_PAGE_ACTION = "DISPLAY_UNIT_PAGE_ACTION";

type DisplayUnitPageActionPayload = {| id: Uuid |};
export type DisplayUnitPageAction = {
  type: DISPLAY_UNIT_PAGE_ACTION,
};

export function buildDisplayUnitPageAction(
  payload: DisplayUnitPageActionPayload,
) {
  return {
    type: "DISPLAY_UNIT_PAGE_ACTION",
    payload,
  };
}

type DisplayUserPageActionPayload = {| id: Uuid |};
export type DisplayUserPageAction = {
  type: DISPLAY_USER_PAGE_ACTION,
  payload: DisplayUserPageActionPayload,
};

export function buildDisplayUserPageAction(
  payload: DisplayUserPageActionPayload,
) {
  return {
    type: "DISPLAY_USER_PAGE_ACTION",
    payload: payload,
  };
}

export type PageAction = DisplayUnitPageAction | DisplayUserPageAction;
