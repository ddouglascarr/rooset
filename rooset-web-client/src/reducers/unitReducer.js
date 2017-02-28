// @flow
import type { Action } from "../actions/Action";
import { UnitState } from "./models/UnitState";
import type {
  UnitQueryResponse,
  UnitQueryRequest,
  UnitQueryError,
  UnitMemberQueryResponse,
} from "../actions/http/QueryActions";
import { fromJS } from "immutable";

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
    case "UNIT_MEMBER_QUERY_RESPONSE":
      return unitMemberQueryResponse(state, action);
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
    .setDescription(action.payload.description)
    .setAreas(fromJS(action.payload.areas));

  return state.setStatus("READY").setPayload(payload);
}

function unitMemberQueryResponse(
  state,
  action: UnitMemberQueryResponse,
): UnitState {
  const members = fromJS(action.payload.members.map(m => m.memberId));
  return state.setPayload(
    state.payload
      .setMembers(members)
      .setTotalWeight(action.payload.totalWeight),
  );
}
