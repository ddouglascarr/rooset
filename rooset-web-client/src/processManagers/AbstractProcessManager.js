// @flow

import type { Action } from "../actions/Action";
import type { Middleware, MiddlewareAPI, Dispatch } from "redux";
import type { State } from "../reducers/rootReducer";

export interface IProcessManager {
  respond(State, Dispatch<Action>, Action): void,
  getMiddleware(): Middleware<State, Action>,
}

export class AbstractProcessManager implements IProcessManager {
  respond(state: State, dispatch: Dispatch<Action>, action: Action): void {}

  getMiddleware(): Middleware<State, Action> {
    return buildProcessManagerMiddleware(this);
  }
}

function buildProcessManagerMiddleware(
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
