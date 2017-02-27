// @flow

import type { DisplayUnitPageAction } from "../actions/PageAction";
import type { Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import { buildUnitQueryRequest } from "../actions/http/QueryActions";
import type {
  IProcessManager,
} from "../processManagers/AbstractProcessManager";
import {
  AbstractProcessManager,
} from "../processManagers/AbstractProcessManager";

export default class PageProcessManager
  extends AbstractProcessManager
  implements IProcessManager {
  respond(state: State, dispatch: Dispatch<Action>, action: Action): void {
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
}
