// @flow

import type {DisplayUnitPageAction } from "../actions/PageAction";
import type { MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";


export function pageProcessManager(store: MiddlewareAPI<State, Action>):
  Dispatch<Action> => Dispatch<Action> {
  return (next) =>
  (action) => {
    switch(action.type) {
      case "DISPLAY_UNIT_PAGE_ACTION":
        displayUnitPage(action);
        break;
      default: // nothing, want to return regarless of dispatch.
    }

    return next(action);
  };

  function displayUnitPage(action: DisplayUnitPageAction) {
    console.log("Unit Page displayed");
  }
}
