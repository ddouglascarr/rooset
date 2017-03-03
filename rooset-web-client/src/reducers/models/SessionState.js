// @flow
/* eslint  no-use-before-define:0 */

import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";
import type { Uuid } from "../../types";

export type SessionStateDataModelType = {
  id: Uuid,
  username: string,
  displayName: string,
}
export type SessionStateModelType = {
  status: "LOGGED_IN" | "REQUESTING" | "LOGGED_OUT" | "ERROR",
  data?: SessionStateDataModelType,
  errorMessage?: string,
};

// /////////////////////////////////////////////////////////////////////////////
//
// NOTE: THIS CLASS IS GENERATED. DO NOT MAKE CHANGES HERE.
//
// If you need to update this class, update the corresponding flow type above
// and re-run the flow-immutable-models codemod
//
// /////////////////////////////////////////////////////////////////////////////
export class SessionState extends ImmutableModel {
  static fromJS(json: SessionStateModelType): SessionState {
    const state: Object = Object.assign({}, json);
    state.data = (state.data ? SessionStateData.fromJS(state.data) : state.data);
    return new this(Immutable.Map(state));
  }

  toJS(): SessionStateModelType {
    const js: SessionStateModelType = {
      status: this.status,
    };

    if (this.data != null) {
      js.data = (this.data ? this.data.toJS() : this.data);
    }

    if (this.errorMessage != null) {
      js.errorMessage = this.errorMessage;
    }

    return js;
  }

  get status(): 'LOGGED_IN' | 'REQUESTING' | 'LOGGED_OUT' | 'ERROR' {
    return this._state.get('status');
  }

  setStatus(status: 'LOGGED_IN' | 'REQUESTING' | 'LOGGED_OUT' | 'ERROR'): this {
    return this.clone(this._state.set('status', status));
  }

  get data(): ?SessionStateData {
    return this._state.get('data');
  }

  setData(data: ?SessionStateData): this {
    return this.clone(this._state.set('data', data));
  }

  get errorMessage(): ?string {
    return this._state.get('errorMessage');
  }

  setErrorMessage(errorMessage: ?string): this {
    return this.clone(this._state.set('errorMessage', errorMessage));
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
export class SessionStateData extends ImmutableModel {
  static fromJS(json: SessionStateDataModelType): SessionStateData {
    const state: Object = Object.assign({}, json);
    return new this(Immutable.Map(state));
  }

  toJS(): SessionStateDataModelType {
    return {
      id: this.id,
      username: this.username,
      displayName: this.displayName,
    };
  }

  get id(): Uuid {
    return this._state.get('id');
  }

  setId(id: Uuid): this {
    return this.clone(this._state.set('id', id));
  }

  get username(): string {
    return this._state.get('username');
  }

  setUsername(username: string): this {
    return this.clone(this._state.set('username', username));
  }

  get displayName(): string {
    return this._state.get('displayName');
  }

  setDisplayName(displayName: string): this {
    return this.clone(this._state.set('displayName', displayName));
  }
}
