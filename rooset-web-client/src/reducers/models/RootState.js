// @flow

import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";

import type { UnitStateModelType } from "./UnitState.js";
import { UnitState } from "./UnitState.js";

export type RootStateModelType = {
  unit: UnitStateModelType,
};

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class RootState extends ImmutableModel {
  static fromJS(json: RootStateModelType): RootState {
    const state: Object = Object.assign({}, json);
    state.unit = UnitState.fromJS(state.unit);
    return new this(Immutable.Map(state));
  }

  toJS(): RootStateModelType {
    return {
      unit: this.unit.toJS()
    };
  }

  get unit(): UnitState {
    return this._state.get("unit");
  }

  setUnit(unit: UnitState): this {
    return this.clone(this._state.set("unit", unit));
  }
}
