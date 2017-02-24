// @flow

import { createStore, applyMiddleware } from "redux";
import createLogger from "redux-logger";
import createSagaMiddleware from "redux-saga";
import rootReducer, { initialState } from "./reducers/rootReducer";

// eslint-disable-next-line no-unused-vars
const stateTransformer = state => {
  if (typeof state.toJS === "function") return state.toJS();
  return state;
};

// eslint-disable-next-line no-unused-vars
const stateTransformerStub = () => {
  return {};
};

export default function configureStore() {
  const loggerMiddleware = createLogger({ collapsed: true, stateTransformer });
  const sagaMiddleware = createSagaMiddleware();
  const store = createStore(
    rootReducer,
    initialState,
    applyMiddleware(loggerMiddleware, sagaMiddleware),
  );
  // sagaMiddleware.run(rootSaga);
  return store;
}
