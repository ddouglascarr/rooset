// @flow
import type { Action } from "../actions/Action";
import { SessionState } from "./models/SessionState";
import type {
  LoginRequestAction,
  LoginErrorAction,
} from "../actions/SessionAction";

export default function sessionReducer(
  state: SessionState,
  action: Action,
): SessionState {
  switch (action.type) {
    case "LOGIN_REQUEST":
      return loginRequest(state, action);
    case "LOGIN_ERROR":
      return loginError(state, action);
    default:
      return state;
  }
}

function loginRequest(
  state: SessionState,
  action: LoginRequestAction,
): SessionState {
  return state.setStatus("REQUESTING").setUsername(action.payload.username);
}

function loginError(
  state: SessionState,
  action: LoginErrorAction,
): SessionState {
  const { message } = action.payload;
  return state.setStatus("ERROR").setErrorMessage(message ? message : "");
}
