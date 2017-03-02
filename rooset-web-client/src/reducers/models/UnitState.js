// @flow
/* eslint-disable no-use-before-define */
import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";
import type { Uuid } from '../../types';

export type AreaModelType = {
  areaId: Uuid,
  name: string,
  description: string,
};

export type UnitStateDataModelType = {
  id: Uuid | null,
  name: string | null,
  description: string | null,
  totalWeight: number,
  members:  Array<Uuid>,
  areas: Array<AreaModelType>,
};

export type UnitStateModelType = {
  status: "EMPTY" | "LOADING" | "READY" | "ERROR",
  error: Object,
  data: UnitStateDataModelType,
}

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class Area extends ImmutableModel {
  static fromJS(json: AreaModelType): Area {
    const state: Object = Object.assign({}, json);
    return new this(Immutable.Map(state));
  }

  toJS(): AreaModelType {
    return {
      areaId: this.areaId,
      name: this.name,
      description: this.description,
    };
  }

  get areaId(): Uuid {
    return this._state.get('areaId');
  }

  setAreaId(areaId: Uuid): this {
    return this.clone(this._state.set('areaId', areaId));
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

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class UnitStateData extends ImmutableModel {
  static fromJS(json: UnitStateDataModelType): UnitStateData {
    const state: Object = Object.assign({}, json);
    state.members = Immutable.List(state.members);
    state.areas = Immutable.List(state.areas).map(item => Area.fromJS(item));
    return new this(Immutable.Map(state));
  }

  toJS(): UnitStateDataModelType {
    return {
      id: this.id,
      name: this.name,
      description: this.description,
      totalWeight: this.totalWeight,
      members: this.members.toArray(),
      areas: this.areas.toArray().map(item => item.toJS()),
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

  get totalWeight(): number {
    return this._state.get('totalWeight');
  }

  setTotalWeight(totalWeight: number): this {
    return this.clone(this._state.set('totalWeight', totalWeight));
  }

  get members(): Immutable.List<Uuid> {
    return this._state.get('members');
  }

  setMembers(members: Immutable.List<Uuid>): this {
    return this.clone(this._state.set('members', members));
  }

  get areas(): Immutable.List<Area> {
    return this._state.get('areas');
  }

  setAreas(areas: Immutable.List<Area>): this {
    return this.clone(this._state.set('areas', areas));
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
    state.data = UnitStateData.fromJS(state.data);
    return new this(Immutable.Map(state));
  }

  toJS(): UnitStateModelType {
    return {
      status: this.status,
      error: this.error,
      data: this.data.toJS(),
    };
  }

  get status(): 'EMPTY' | 'LOADING' | 'READY' | 'ERROR' {
    return this._state.get('status');
  }

  setStatus(status: 'EMPTY' | 'LOADING' | 'READY' | 'ERROR'): this {
    return this.clone(this._state.set('status', status));
  }

  get error(): Object {
    return this._state.get('error');
  }

  setError(error: Object): this {
    return this.clone(this._state.set('error', error));
  }

  get data(): UnitStateData {
    return this._state.get('data');
  }

  setData(data: UnitStateData): this {
    return this.clone(this._state.set('data', data));
  }
}
