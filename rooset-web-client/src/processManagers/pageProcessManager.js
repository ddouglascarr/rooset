// @flow

import type {DisplayUnitPageAction } from "../actions/PageAction";
import type { Store } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";


export const pageProcessManager = (store: Store<State, Action>) =>
  (next: () => void) =>
  (action: Action) => {
    next(action);
    switch(action.type) {
      case "DISPLAY_UNIT_PAGE_ACTION":
        return displayUnitPage(action);
      default:
        return next(action);
    }

  function displayUnitPage(action: DisplayUnitPageAction) {
    next(action);
  }


  };
