import { createStore, applyMiddleware } from "redux";
import createLogger from "redux-logger";
import createSagaMiddleware from "redux-saga";
import rootReducer from "./reducers/rootReducer";
import { Map } from "immutable";

export default function configureStore(initialState) {
  const loggerMiddleware = createLogger({ collapsed: true });
  const sagaMiddleware = createSagaMiddleware();
  const store = createStore(
    rootReducer,
    Map(), // initial state
    applyMiddleware(loggerMiddleware, sagaMiddleware),
  );
  // sagaMiddleware.run(rootSaga);
  return store;
}
