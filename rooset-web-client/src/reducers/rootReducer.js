// @flow

import type { Action } from "../actions/Action";
import { Root } from "./models/Root";

export const initialState: Root = Root.fromJS({
  unit: { status: "EMPTY", payload: { id: null, name: null, description: null } },
});
console.log(initialState);
export default function(state: Root, action: Action): Root {
  return state;
}
