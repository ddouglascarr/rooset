// @flow
jest.mock("../../services/sessionService");

import SessionProcessManager from "../SessionProcessManager";
const performLoginRequest: any = require(
  "../../services/sessionService",
).performLoginRequest;
const getSession: any = require("../../services/sessionService").getSession;
import {
  buildLoginRequestAction,
  buildLoginResponseAction,
  buildSessionErrorAction,
  buildSessionRequestAction,
  buildSessionResponseAction,
} from "../../actions/SessionAction";
import { initialState } from "../../reducers/rootReducer";
const dispatch = jest.fn();

describe("sessionProcessManager", () => {
  it("LOGIN_REQUEST", async () => {
    performLoginRequest.mockReturnValue(
      Promise.resolve({ username: "testuser" }),
    );
    const sessionProcessManager = new SessionProcessManager();
    await sessionProcessManager.respond(
      initialState,
      dispatch,
      buildLoginRequestAction({
        username: "testuser",
        password: "password1",
      }),
    );
    expect(performLoginRequest).toHaveBeenCalledWith("testuser", "password1");
    expect(dispatch).toHaveBeenCalledWith(
      buildLoginResponseAction({ username: "testuser" }),
    );
  });

  it("SESSION_REQUEST session is active", async () => {
    getSession.mockReturnValueOnce(
      Promise.resolve({
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      }),
    );
    const sessionProcessManager = new SessionProcessManager();
    await sessionProcessManager.respond(
      initialState,
      dispatch,
      buildSessionRequestAction(),
    );

    expect(getSession).toHaveBeenCalled();
    expect(dispatch).toHaveBeenCalledWith(
      buildSessionResponseAction({
        displayName: "Determined Poitras",
        id: "464b1ebb-32c1-460c-8e9e-444444444444",
        username: "determinedPoitras",
      }),
    );
  });

  it("SESSION_REQUEST session is inactive", async () => {
    getSession.mockReturnValueOnce(Promise.resolve(null));
    const sessionProcessManager = new SessionProcessManager();
    await sessionProcessManager.respond(
      initialState,
      dispatch,
      buildSessionRequestAction(),
    );

    expect(getSession).toHaveBeenCalled();
    expect(dispatch).toHaveBeenCalledWith(buildSessionErrorAction());
  });
});
