// @flow

import React from "react";
import { connect } from "react-redux";
import { UnitState } from "../reducers/models/UnitState";
import { RootState } from "../reducers/models/RootState";
import type { Action } from "../actions/Action";

type Props = {
  unit: UnitState,
  dispatch: (Action) => void,
  match: Object,
};

class UnitPage extends React.Component {
  props: Props;
  render() {
    const { match, unit } = this.props;
    return <div>Unit Page: {unit.status} {match.params.unitId}</div>;
  }
}

const mapState = (state: RootState) => {
  return {
    unit: state.unit,
  };
};

export { UnitPage };
export default connect(mapState)(UnitPage);
