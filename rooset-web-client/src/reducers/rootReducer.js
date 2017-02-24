// @flow

import type { Action } from "../actions/Action";
import { Root } from "./models/Root";
import { Unit } from "./models/Unit";

const initialState: Root = Root.fromJS({
  unit: { id: "", name: "", description: "" },
});

export default function(state: Root = initialState, action: Action): Root {
  return state;
}
