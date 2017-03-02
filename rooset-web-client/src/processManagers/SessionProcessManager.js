// @flow
import type { Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import type { LoginRequestAction } from "../actions/SessionAction";
import {
  buildLoginResponseAction,
  buildLoginErrorAction,
} from "../actions/SessionAction";
import { performLoginRequest } from "../services/sessionService";

import type { IProcessManager } from "../processManagers/ProcessManager";

export default class SessionProcessManager implements IProcessManager {
  respond(
    state: State,
    dispatch: Dispatch<Action>,
    action: Action,
  ): Promise<void> {
    switch (action.type) {
      case "LOGIN_REQUEST":
        return loginRequest(state, dispatch, action);
      default:
        return Promise.resolve(); // nothing
    }
  }
}

async function loginRequest(
  state,
  dispatch,
  action: LoginRequestAction,
): Promise<void> {
  const { username, password } = action.payload;
  try {
    await performLoginRequest(username, password);
    dispatch(buildLoginResponseAction({ username }));
  } catch (err) {
    dispatch(
      buildLoginErrorAction({
        message: err.message || "Wrong username / password",
      }),
    );
  }
  return Promise.resolve();
}
