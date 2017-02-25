// @flow
/* eslint-disable no-use-before-define */
import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";
import type { Uuid } from '../../types';

export type UnitStatePayloadModelType = {
  id: Uuid | null,
  name: string | null,
  description: string | null,
};

export type UnitStateModelType = {
  status: "EMPTY" | "LOADING" | "READY" | "ERROR",
  error: Array<any>,
  payload: UnitStatePayloadModelType,
}

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class UnitStatePayload extends ImmutableModel {
  static fromJS(json: UnitStatePayloadModelType): UnitStatePayload {
    const state: Object = Object.assign({}, json);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitStatePayloadModelType {
    return {
      id: this.id,
      name: this.name,
      description: this.description,
    };
  }

  get id(): Uuid | null {
    return this._state.get('id');
  }

  setId(id: Uuid | null): this {
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

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class UnitState extends ImmutableModel {
  static fromJS(json: UnitStateModelType): UnitState {
    const state: Object = Object.assign({}, json);
    state.error = Immutable.List(state.error);
    state.payload = UnitStatePayload.fromJS(state.payload);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitStateModelType {
    return {
      status: this.status,
      error: this.error.toArray(),
      payload: this.payload.toJS(),
    };
  }

  get status(): 'EMPTY' | 'LOADING' | 'READY' | 'ERROR' {
    return this._state.get('status');
  }

  setStatus(status: 'EMPTY' | 'LOADING' | 'READY' | 'ERROR'): this {
    return this.clone(this._state.set('status', status));
  }

  get error(): Immutable.List<any> {
    return this._state.get('error');
  }

  setError(error: Immutable.List<any>): this {
    return this.clone(this._state.set('error', error));
  }

  get payload(): UnitStatePayload {
    return this._state.get('payload');
  }

  setPayload(payload: UnitStatePayload): this {
    return this.clone(this._state.set('payload', payload));
  }
}
