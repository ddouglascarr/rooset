// @flow
import type { Store } from "redux";
import type { State } from "./reducers/rootReducer";
import type { Action } from "./actions/Action";
import { createStore, applyMiddleware } from "redux";
import createLogger from "redux-logger";
import rootReducer, { initialState } from "./reducers/rootReducer";
import PageProcessManager from "./processManagers/PageProcessManager";
import SessionProcessManager from "./processManagers/SessionProcessManager";
import {
  httpRequestProcessManager,
} from "./processManagers/httpRequestProcessManager";
import {
  buildProcessManagerMiddleware,
} from "./processManagers/ProcessManager";

// eslint-disable-next-line no-unused-vars
const stateTransformer = state => {
  if (typeof state.toJS === "function") return state.toJS();
  return state;
};

// eslint-disable-next-line no-unused-vars
const stateTransformerStub = () => {
  return {};
};

export default function configureStore(): Store<State, Action> {
  const loggerMiddleware = createLogger({ collapsed: true, stateTransformer });
  const store: Store<State, Action> = createStore(
    rootReducer,
    initialState,
    applyMiddleware(
      loggerMiddleware,
      buildProcessManagerMiddleware(new PageProcessManager()),
      buildProcessManagerMiddleware(new SessionProcessManager()),
      httpRequestProcessManager,
    ),
  );
  return store;
}
