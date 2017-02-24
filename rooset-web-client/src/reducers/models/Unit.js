// @flow

import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";

export type UnitModelType = {
  id: string,
  name: string,
  description: string,
};

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class Unit extends ImmutableModel {
  static fromJS(json: UnitModelType): Unit {
    const state: Object = Object.assign({}, json);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitModelType {
    return {
      id: this.id,
      name: this.name,
      description: this.description,
    };
  }

  get id(): string {
    return this._state.get('id');
  }

  setId(id: string): this {
    return this.clone(this._state.set('id', id));
  }

  get name(): string {
    return this._state.get('name');
  }

  setName(name: string): this {
    return this.clone(this._state.set('name', name));
  }

  get description(): string {
    return this._state.get('description');
  }

  setDescription(description: string): this {
    return this.clone(this._state.set('description', description));
  }
}
