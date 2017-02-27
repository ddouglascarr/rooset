// @flow

import type { Action } from "../actions/Action";
import type { Middleware, MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";

export interface IProcessManager {
  respond(State, Dispatch<Action>, Action): void,
}

export class AbstractProcessManager {

}

export function buildProcessManagerMiddleware(
  manager: IProcessManager,
): Middleware<State, Action> {
  return function(
    store: MiddlewareAPI<State, Action>,
  ): (Dispatch<Action>) => Dispatch<Action> {
    return next => action => {
      manager.respond(store.getState(), store.dispatch, action);
      return next(action);
    };
  };
}
