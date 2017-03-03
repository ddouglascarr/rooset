// @flow
import type { Uuid } from "../types";

export type LOGIN_REQUEST = "LOGIN_REQUEST";
export type LOGIN_ERROR = "LOGIN_ERROR";
export type SESSION_REQUEST = "SESSION_REQUEST";
export type SESSION_RESPONSE = "SESSION_RESPONSE";
export type SESSION_ERROR = "SESSION_ERROR";
export type SESSION_END = "SESSION_END";

type LoginRequestActionPayload = {| username: string, password: string |};
export type LoginRequestAction = {
  type: LOGIN_REQUEST,
  payload: LoginRequestActionPayload,
};

export function buildLoginRequestAction(payload: LoginRequestActionPayload) {
  return {
    type: "LOGIN_REQUEST",
    payload,
  };
}

export type LoginErrorActionPayload = {
  message: string,
};
export type LoginErrorAction = {|
  type: LOGIN_ERROR,
  payload: LoginErrorActionPayload,
|};

export function buildLoginErrorAction(payload: LoginErrorActionPayload) {
  return { type: "LOGIN_ERROR", payload };
}

export type SessionRequestAction = {|
  type: SESSION_REQUEST,
|};
export function buildSessionRequestAction(): SessionRequestAction {
  return { type: "SESSION_REQUEST" };
}

export type SessionErrorAction = {|
  type: "SESSION_ERROR",
|};
export function buildSessionErrorAction(): SessionErrorAction {
  return { type: "SESSION_ERROR" };
}

type SessionResponseActionPayload = {
  id: Uuid,
  displayName: string,
  username: string,
};
export type SessionResponseAction = {|
  type: SESSION_RESPONSE,
  payload: SessionResponseActionPayload,
|};
export function buildSessionResponseAction(
  payload: SessionResponseActionPayload,
): SessionResponseAction {
  return {
    type: "SESSION_RESPONSE",
    payload,
  };
}

type SessionEndAction = {|
  type: SESSION_END,
|};
export function buildSessionEndAction(): SessionEndAction {
  return { type: "SESSION_END" };
}

export type SessionAction =
  | LoginRequestAction
  | LoginErrorAction
  | SessionRequestAction
  | SessionErrorAction
  | SessionResponseAction
  | SessionEndAction;
