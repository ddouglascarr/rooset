// @flow

import type { DisplayUnitPageAction } from "../actions/PageAction";
import type { Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import {
  buildUnitQueryRequest,
  buildUnitQueryError,
  buildUnitMemberQueryRequest,
} from "../actions/http/QueryActions";
import type { IProcessManager } from "../processManagers/ProcessManager";
import { AbstractProcessManager } from "../processManagers/ProcessManager";
import { getUnitUrlParameterName } from "../services/urlParameterService";

export default class PageProcessManager
  extends AbstractProcessManager
  implements IProcessManager {
  respond(state: State, dispatch: Dispatch<Action>, action: Action): Promise<void> {
    switch (action.type) {
      case "DISPLAY_UNIT_PAGE_ACTION":
        return displayUnitPage(state, dispatch, action);
      default:
        return Promise.resolve();// nothing, want to return regarless of dispatch.
    }
  }
}

async function displayUnitPage(state, dispatch, action: DisplayUnitPageAction) {
  console.log("Unit Page displayed");
  const unitId = await getUnitUrlParameterName(action.payload.urlParameterName);
  if (!unitId) {
    dispatch(
      buildUnitQueryError({
        code: "QUERY_EXCEPTION",
        message: `no unit at ${action.payload.urlParameterName}`,
      }),
    );
    return Promise.resolve();
  }
  dispatch(buildUnitQueryRequest({ id: unitId }));
  dispatch(buildUnitMemberQueryRequest({ id: unitId }));
  return Promise.resolve();
}
