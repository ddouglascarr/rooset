import httpRequestSaga from "./httpRequestSaga";

export default function* rootSaga() {
  yield [httpRequestSaga()];
}
