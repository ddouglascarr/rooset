// @flow

import type { Action } from "../actions/Action";
import { RootState } from "./models/RootState";
export type State = RootState;
import unitReducer from "./unitReducer";
import sessionReducer from "./sessionReducer";

export const initialState: RootState = RootState.fromJS({
  unit: {
    status: "EMPTY",
    data: {
      id: null,
      name: null,
      description: null,
      members: [],
      areas: [],
      totalWeight: 0,
    },
    error: {},
  },
  session: { status: "LOGGED_OUT", username: "" },
});

export default function(state: RootState, action: Action): RootState {
  return state
    .setUnit(unitReducer(state.unit, action))
    .setSession(sessionReducer(state.session, action));
}
