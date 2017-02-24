// @flow

import type { Action } from "../actions/Action";
import { RootState } from "./models/RootState";

export const initialState: RootState = RootState.fromJS({
  unit: {
    status: "EMPTY",
    payload: { id: null, name: null, description: null },
    error: [],
  },
});

export default function(state: RootState, action: Action): RootState {
  return state;
}
