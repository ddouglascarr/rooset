// @flow

import type { DisplayUnitPageAction } from "../actions/PageAction";
import type { Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import { buildUnitQueryRequest, buildUnitQueryError } from "../actions/http/QueryActions";
import type {
  IProcessManager,
} from "../processManagers/ProcessManager";
import {
  AbstractProcessManager,
} from "../processManagers/ProcessManager";
import { getUnitUrlParameterName } from "../services/urlParameterService";

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

    async function displayUnitPage(action: DisplayUnitPageAction) {
      console.log("Unit Page displayed");
      const unitId = await getUnitUrlParameterName(action.payload.urlParameterName);
      if (!unitId) {
        dispatch(buildUnitQueryError({
          code: "QUERY_EXCEPTION",
          message: `no unit at ${action.payload.urlParameterName}`
        }));
        return;
      }
      dispatch(buildUnitQueryRequest({ id: unitId }));
    }
  }
}
