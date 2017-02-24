// @flow

import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";

import type { UnitModelType } from "./Unit.js";
import { Unit } from "./Unit.js";

export type RootModelType = {
  unit: UnitModelType,
};

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class Root extends ImmutableModel {
  static fromJS(json: RootModelType): Root {
    const state: Object = Object.assign({}, json);
    state.unit = Unit.fromJS(state.unit);
    return new this(Immutable.Map(state));
  }

  toJS(): RootModelType {
    return {
      unit: this.unit.toJS(),
    };
  }

  get unit(): Unit {
    return this._state.get("unit");
  }

  setUnit(unit: Unit): this {
    return this.clone(this._state.set("unit", unit));
  }
}
