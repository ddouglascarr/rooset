// @flow
import type { Store } from "redux";
import type { State } from "./reducers/rootReducer";
import type { Action } from "./actions/Action";
import { createStore, applyMiddleware } from "redux";
import createLogger from "redux-logger";
// import createSagaMiddleware from "redux-saga";
import rootReducer, { initialState } from "./reducers/rootReducer";
// import rootSaga from "./sagas/rootSaga";
import PageProcessManager from "./processManagers/pageProcessManager";
import {
  httpRequestProcessManager,
} from "./processManagers/httpRequestProcessManager";

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
  //  const sagaMiddleware = createSagaMiddleware();
  const store: Store<State, Action> = createStore(
    rootReducer,
    initialState,
    applyMiddleware(
      loggerMiddleware,
      // sagaMiddleware,
      new PageProcessManager().getMiddleware(),
      httpRequestProcessManager,
    ),
  );
  // sagaMiddleware.run(rootSaga);
  return store;
}
