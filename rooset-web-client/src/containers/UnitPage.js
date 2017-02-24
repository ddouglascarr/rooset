// @flow

import React from "react";
import { connect } from "react-redux";
import { UnitState } from "../reducers/models/UnitState";
import { RootState } from "../reducers/models/RootState";

class UnitPage extends React.Component {
  static propTypes: {
    unit: UnitState,
    dispatch: function,
  };

  render() {}
}

const mapState = (state: RootState) => {
  return {
    unit: state.unit,
  };
};

export { UnitPage };
export default connect(mapState)(UnitPage);
