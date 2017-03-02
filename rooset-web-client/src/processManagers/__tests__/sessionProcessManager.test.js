// @flow
jest.mock("../../services/sessionService");

import SessionProcessManager from "../sessionProcessManager";
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
  it("LOGIN_REQUEST", () => {
    performLoginRequest.mockReturnValue(
      Promise.resolve({ username: "testuser" }),
    );
    const sessionProcessManager = new SessionProcessManager();
    return sessionProcessManager
      .respond(
        initialState,
        dispatch,
        buildLoginRequestAction({
          username: "testuser",
          password: "password1",
        }),
      )
      .then(() => {
        expect(performLoginRequest).toHaveBeenCalledWith(
          "testuser",
          "password1",
        );
        expect(dispatch).toHaveBeenCalledWith(
          buildLoginResponseAction({ username: "testuser" }),
        );
      });
  });
});
