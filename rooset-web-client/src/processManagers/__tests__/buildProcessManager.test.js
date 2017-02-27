// @flow
import type { IProcessManager } from "../AbstractProcessManager";
import { AbstractProcessManager } from "../AbstractProcessManager";
import { initialState } from "../../reducers/rootReducer";
import { buildLoginErrorAction } from "../../actions/SessionAction";
import { createStore } from "redux";
import type { Action } from "../../actions/Action";

describe("buildProcessManager", () => {
  const mockState = initialState;
  const mockStore = createStore(s => s, initialState);
  const mockNext = jest.fn((action: Action) => action);
  const mockAction = buildLoginErrorAction();
  it("should return process manager middleware", () => {
    class MockProcessManager extends AbstractProcessManager implements IProcessManager {
      respond = jest.fn((state, dispatch, action) => {});
    }
    const mockProcessManager = new MockProcessManager();
    const middleware = mockProcessManager.getMiddleware();
    const wrapper = middleware(mockStore);
    const dispatcher = wrapper(mockNext);
    const result = dispatcher(mockAction);
    expect(mockNext).toHaveBeenCalled();
    expect(mockProcessManager.respond).toHaveBeenCalledTimes(1);
    expect(mockProcessManager.respond.mock.calls[0].length).toEqual(3);
    const args = mockProcessManager.respond.mock.calls[0];
    expect(args[0]).toEqual(mockStore.getState());
    expect(args[1]).toEqual(mockStore.dispatch);
    expect(args[2]).toEqual(mockAction);
  });
});
