// @flow
import type { Uuid } from "../types";
import executeHttpRequest from "../actions/http/executeHttpRequest";
import {
  buildUnitUrlParameterNameQueryRequest,
} from "../actions/http/QueryActions";

const unitUrlParameterNameCache: Map<string, Uuid> = new Map();
export async function getUnitUrlParameterName(
  urlParameterName: string,
): Promise<?Uuid> {
  if (unitUrlParameterNameCache.has(urlParameterName)) {
    return unitUrlParameterNameCache.get(urlParameterName);
  }
  const req = buildUnitUrlParameterNameQueryRequest({ urlParameterName });
  const resp = await executeHttpRequest(req);
  if (resp.type === "UNIT_URL_PARAMETER_NAME_QUERY_RESPONSE") {
    return resp.payload.id;
  }
  throw new Error(`no unitUrlParameterName for ${urlParameterName}`);
}
