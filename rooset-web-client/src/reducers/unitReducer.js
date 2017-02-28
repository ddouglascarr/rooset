// @flow
import type { Action } from "../actions/Action";
import { UnitState } from "./models/UnitState";
import type {
  UnitQueryResponse,
  UnitQueryRequest,
  UnitQueryError,
} from "../actions/http/QueryActions";

export default function unitReducer(
  state: UnitState,
  action: Action,
): UnitState {
  switch (action.type) {
    case "UNIT_QUERY_REQUEST":
      return unitQueryRequest(state, action);
    case "UNIT_QUERY_ERROR":
      return unitQueryError(state, action);
    case "UNIT_QUERY_RESPONSE":
      return unitQueryResponse(state, action);
    default:
      return state;
  }
}

function unitQueryRequest(
  state: UnitState,
  action: UnitQueryRequest,
): UnitState {
  return state.setStatus("LOADING");
}

function unitQueryError(state: UnitState, action: UnitQueryError): UnitState {
  return state.setStatus("ERROR");
}

function unitQueryResponse(
  state: UnitState,
  action: UnitQueryResponse,
): UnitState {
  const payload = state.payload
    .setId(action.payload.id)
    .setName(action.payload.name)
    .setDescription(action.payload.description);

  return state
    .setStatus("READY")
    .setPayload(payload);
}
