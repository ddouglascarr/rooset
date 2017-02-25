// @flow

import type { Action } from "../actions/Action";
import executeHttpRequest from "../actions/http/executeHttpRequest";
import type { Store } from "redux";
import type { State } from "../reducers/rootReducer";

// @flow
export const httpRequestProcessManager = (store: Store<State, Action>) => (next: () => void) => (
  action: Action,
) => {
  next(action);
  if (action.meta && action.meta.isHttpRequest) {
    executeHttpRequest(action).then(store.dispatch);
  }
};
