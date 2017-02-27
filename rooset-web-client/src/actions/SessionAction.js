// @flow

export type LOGIN_REQUEST = "LOGIN_REQUEST";
export type LOGIN_RESPONSE = "LOGIN_RESPONSE";
export type LOGIN_ERROR = "LOGIN_ERROR";

type LoginRequestActionPayload = {| username: String, password: String |};
export type LoginRequestAction = {
  type: LOGIN_REQUEST,
  payload: LoginRequestActionPayload,
};

export function buildLoginRequestActionPayload(
  payload: LoginRequestActionPayload,
) {
  return {
    type: "LOGIN_REQUEST",
    payload,
  };
}

type LoginResponseActionPayload = {| username: String |};
export type LoginResponseAction = {|
  type: LOGIN_RESPONSE,
  payload: LoginResponseActionPayload,
|};

export function buildLoginResponseAction(payload: LoginResponseActionPayload) {
  return {
    type: "LOGIN_RESPONSE",
    payload,
  };
}

export type LoginErrorAction = {|
  type: LOGIN_ERROR,
  payload: {},
|};

export function buildLoginErrorAction() {
  return { type: "LOGIN_ERROR", payload: {} };
}

export type SessionAction =
  | LoginRequestAction
  | LoginErrorAction
  | LoginResponseAction;
