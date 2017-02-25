import { takeEvery } from "redux-saga";
import { put, call } from "redux-saga/effects";

const takePredicate = action => action.isHttpQueryAction;
export default function* watcher() {
  yield* takeEvery(takePredicate, dispatchRequest);
}

export function* dispatchRequest(action) {
  const req = action.getHttpRequest();
  const resp = yield call([req, req.execute]);
  const body = yield call(resp.json);
  if (!resp.ok) return action.buildError(body.payload);
  return action.buildResponse(body.payload);
}
