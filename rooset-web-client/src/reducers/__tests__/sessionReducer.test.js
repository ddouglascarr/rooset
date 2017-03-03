// @flow
import sessionReducer from "../sessionReducer";
import {
  buildLoginRequestAction,
  buildLoginErrorAction,
  buildSessionRequestAction,
  buildSessionErrorAction,
  buildSessionResponseAction,
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
      errorMessage: "wrong username / password",
    });
  });

  it("handle SESSION_REQUEST", () => {
    const prevState = mockState;
    const newState = sessionReducer(prevState, buildSessionRequestAction());
    expect(newState.toJS()).toEqual({
      status: "REQUESTING",
    });
  });

  it("handle SESSION_ERROR", () => {
    const prevState = mockState.setStatus("REQUESTING");
    const newState = sessionReducer(prevState, buildSessionErrorAction());
    expect(newState.toJS()).toEqual({
      status: "LOGGED_OUT",
    });
  });

  it("handle SESSION_RESPONSE", () => {
    const prevState = mockState.setStatus("REQUESTING");
    const newState = sessionReducer(
      prevState,
      buildSessionResponseAction({
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      }),
    );
    expect(newState.toJS()).toEqual({
      status: "LOGGED_IN",
      data: {
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      },
    });
  });

  it("SESSION_RESPONSE should clear errorMessage", () => {
    const prevState = mockState
      .setStatus("REQUESTING")
      .setErrorMessage("Wrong username / password");
    const newState = sessionReducer(
      prevState,
      buildSessionResponseAction({
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      }),
    );
    expect(newState.toJS()).toEqual({
      status: "LOGGED_IN",
      data: {
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      },
    });
  });
});
