// @flow
/* eslint-disable no-use-before-define */
import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";
import type { uuid } from '../../types';

export type UnitPayloadModelType = {
  id: uuid | null,
  name: string | null,
  description: string | null,
};

export type UnitModelType = {
  status: "EMPTY" | "LOADING" | "READY",
  payload: UnitPayloadModelType,
}

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
    state.payload = UnitPayload.fromJS(state.payload);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitModelType {
    return {
      status: this.status,
      payload: this.payload.toJS(),
    };
  }

  get status(): 'EMPTY' | 'LOADING' | 'READY' {
    return this._state.get('status');
  }

  setStatus(status: 'EMPTY' | 'LOADING' | 'READY'): this {
    return this.clone(this._state.set('status', status));
  }

  get payload(): UnitPayload {
    return this._state.get('payload');
  }

  setPayload(payload: UnitPayload): this {
    return this.clone(this._state.set('payload', payload));
  }
}

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class UnitPayload extends ImmutableModel {
  static fromJS(json: UnitPayloadModelType): UnitPayload {
    const state: Object = Object.assign({}, json);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitPayloadModelType {
    return {
      id: this.id,
      name: this.name,
      description: this.description,
    };
  }

  get id(): uuid | null {
    return this._state.get('id');
  }

  setId(id: uuid | null): this {
    return this.clone(this._state.set('id', id));
  }

  get name(): string | null {
    return this._state.get('name');
  }

  setName(name: string | null): this {
    return this.clone(this._state.set('name', name));
  }

  get description(): string | null {
    return this._state.get('description');
  }

  setDescription(description: string | null): this {
    return this.clone(this._state.set('description', description));
  }
}
