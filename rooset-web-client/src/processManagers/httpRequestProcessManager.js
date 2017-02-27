// @flow

import type { Action } from "../actions/Action";
import executeHttpRequest from "../actions/http/executeHttpRequest";
import type { MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";

// @flow
export function httpRequestProcessManager(store: MiddlewareAPI<State, Action>): (Dispatch<Action>) => Dispatch<Action> {
  return function(next) {
    return function(action) {
      if (action.meta && action.meta.isHttpRequest) {
        executeHttpRequest(action).then(store.dispatch);
      }
      return next(action);
    };
  };
}
