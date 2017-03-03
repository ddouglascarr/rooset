// @flow
import type { Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";
import type { Action } from "../actions/Action";
import type {
  LoginRequestAction,
  SessionRequestAction,
} from "../actions/SessionAction";
import {
  buildLoginErrorAction,
  buildSessionRequestAction,
  buildSessionResponseAction,
  buildSessionErrorAction,
} from "../actions/SessionAction";
import { performLoginRequest, getSession } from "../services/sessionService";

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
      case "SESSION_REQUEST":
        return sessionRequest(state, dispatch, action);
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
    dispatch(buildSessionRequestAction());
  } catch (err) {
    dispatch(
      buildLoginErrorAction({
        message: err.message || "Wrong username / password",
      }),
    );
  }
  return Promise.resolve();
}

async function sessionRequest(
  state,
  dispatch,
  action: SessionRequestAction,
): Promise<void> {
  const session = await getSession();
  if (session) {
    dispatch(
      buildSessionResponseAction({
        id: session.id,
        displayName: session.displayName,
        username: session.username,
      }),
    );
  } else {
    dispatch(buildSessionErrorAction());
  }
}
