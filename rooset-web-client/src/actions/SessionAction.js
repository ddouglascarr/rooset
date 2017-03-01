// @flow

export type LOGIN_REQUEST = "LOGIN_REQUEST";
export type LOGIN_RESPONSE = "LOGIN_RESPONSE";
export type LOGIN_ERROR = "LOGIN_ERROR";

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

type LoginResponseActionPayload = {| username: string |};
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

export type LoginErrorActionPayload = {
  message: string,
}
export type LoginErrorAction = {|
  type: LOGIN_ERROR,
  payload: LoginErrorActionPayload,
|};

export function buildLoginErrorAction(payload: LoginErrorActionPayload) {
  return { type: "LOGIN_ERROR", payload };
}

export type SessionAction =
  | LoginRequestAction
  | LoginErrorAction
  | LoginResponseAction;
