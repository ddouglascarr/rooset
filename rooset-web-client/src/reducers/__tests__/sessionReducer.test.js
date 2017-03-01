// @flow
import sessionReducer from "../sessionReducer";
import {
  buildLoginRequestAction,
  buildLoginResponseAction,
  buildLoginErrorAction,
} from "../../actions/SessionAction";
import { initialState } from "../rootReducer";

const mockState = initialState.session;

describe("sessionReducer", () => {
  it("handle LOGIN_REQUEST", () => {
    const newState = sessionReducer(
      mockState,
      buildLoginRequestAction({
        username: "testuser",
        password: "password1",
      }),
    );

    expect(newState.toJS()).toEqual({
      status: "REQUESTING",
      username: "testuser",
    });
  });

  it("handle LOGIN_RESPONSE", () => {
    const prevState = sessionReducer(
      mockState,
      buildLoginRequestAction({ username: "testuser", password: "password1" }),
    );
    const newState = sessionReducer(
      prevState,
      buildLoginResponseAction({ username: "testuser" }),
    );
    expect(newState.toJS()).toEqual({
      status: "LOGGED_IN",
      username: "testuser",
    });
  });

  it("handle LOGIN_ERROR", () => {
    const prevState = sessionReducer(
      mockState,
      buildLoginRequestAction({ username: "testuser", password: "password1" }),
    );
    const newState = sessionReducer(
      prevState,
      buildLoginErrorAction({ message: "wrong username / password" }),
    );
    expect(newState.toJS()).toEqual({
      status: "ERROR",
      username: "testuser",
      errorMessage: "wrong username / password",
    });
  });
});
