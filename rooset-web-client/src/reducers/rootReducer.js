// @flow

import type { Action } from "../actions/Action";
import { RootState } from "./models/RootState";
export type State = RootState;
import unitReducer from "./unitReducer";

export const initialState: RootState = RootState.fromJS({
  unit: {
    status: "EMPTY",
    payload: { id: null, name: null, description: null },
    error: [],
  },
  session: { status: "LOGGED_OUT", username: "" },
});

export default function(state: RootState, action: Action): RootState {
  return state.setUnit(unitReducer(state.unit, action));
}
