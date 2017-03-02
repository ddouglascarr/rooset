// @flow
jest.mock("../../services/sessionService");

import SessionProcessManager from "../SessionProcessManager";
const performLoginRequest: any = require(
  "../../services/sessionService",
).performLoginRequest;
import {
  buildLoginRequestAction,
  buildLoginResponseAction,
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
});
