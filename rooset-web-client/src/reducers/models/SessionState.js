// @flow
import * as Immutable from "immutable";
import ImmutableModel from "flow-immutable-models";

export type SessionStateModelType = {
  status: "LOGGED_IN" | "REQUESTING" | "LOGGED_OUT",
  username: string,
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
    return new this(Immutable.Map(state));
  }

  toJS(): SessionStateModelType {
    return {
      status: this.status,
      username: this.username,
    };
  }

  get status(): 'LOGGED_IN' | 'REQUESTING' | 'LOGGED_OUT' {
    return this._state.get('status');
  }

  setStatus(status: 'LOGGED_IN' | 'REQUESTING' | 'LOGGED_OUT'): this {
    return this.clone(this._state.set('status', status));
  }

  get username(): string {
    return this._state.get('username');
  }

  setUsername(username: string): this {
    return this.clone(this._state.set('username', username));
  }
}
