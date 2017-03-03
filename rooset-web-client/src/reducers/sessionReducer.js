// @flow
import type { Action } from "../actions/Action";
import { SessionState, SessionStateData } from "./models/SessionState";
import type {
  LoginRequestAction,
  LoginErrorAction,
  SessionRequestAction,
  SessionErrorAction,
  SessionResponseAction,
} from "../actions/SessionAction";
import { fromJS } from "immutable";

export default function sessionReducer(
  state: SessionState,
  action: Action,
): SessionState {
  switch (action.type) {
    case "LOGIN_REQUEST":
      return loginRequest(state, action);
    case "LOGIN_ERROR":
      return loginError(state, action);
    case "SESSION_REQUEST":
      return sessionRequest(state, action);
    case "SESSION_ERROR":
      return sessionError(state, action);
    case "SESSION_RESPONSE":
      return sessionResponse(state, action);
    default:
      return state;
  }
}

function loginRequest(
  state: SessionState,
  action: LoginRequestAction,
): SessionState {
  return state.setStatus("REQUESTING");
}

function loginError(
  state: SessionState,
  action: LoginErrorAction,
): SessionState {
  const { message } = action.payload;
  return state.setStatus("ERROR").setErrorMessage(message ? message : "");
}

function sessionRequest(
  state: SessionState,
  action: SessionRequestAction,
): SessionState {
  return state.setStatus("REQUESTING");
}

function sessionError(
  state: SessionState,
  action: SessionErrorAction,
): SessionState {
  return state.setStatus("LOGGED_OUT");
}

function sessionResponse(
  state: SessionState,
  action: SessionResponseAction,
): SessionState {
  return state
    .setStatus("LOGGED_IN")
    .setData(SessionStateData.fromJS(action.payload))
    .setErrorMessage(null);
}
