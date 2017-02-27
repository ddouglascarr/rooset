// @flow

import type { Action } from "../actions/Action";
import executeHttpRequest from "../actions/http/executeHttpRequest";
import type { Middleware, MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";

export default function buildProcessManager(
  fn: (State, Dispatch<Action>, Action) => void,
): Middleware<State, Action> {
  return function(
    store: MiddlewareAPI<State, Action>,
  ): (Dispatch<Action>) => Dispatch<Action> {
    return next => action => {
      fn(store.getState(), store.dispatch, action);
      return next(action);
    };
  };
}
