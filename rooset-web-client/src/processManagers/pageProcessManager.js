// @flow

import type { DisplayUnitPageAction } from "../actions/PageAction";
import type { MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import { buildUnitQueryRequest } from "../actions/http/QueryActions";

export default function pageProcessManager(
  state: State,
  dispatch: Dispatch<Action>,
  action: Action,
) {
  switch (action.type) {
    case "DISPLAY_UNIT_PAGE_ACTION":
      displayUnitPage(action);
      break;
    default: // nothing, want to return regarless of dispatch.
  }

  function displayUnitPage(action: DisplayUnitPageAction) {
    console.log("Unit Page displayed");
    dispatch(buildUnitQueryRequest(action.payload));
  }
}
