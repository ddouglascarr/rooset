// @flow

import React from "react";
import { connect } from "react-redux";
import { UnitState } from "../reducers/models/UnitState";
import { RootState } from "../reducers/models/RootState";
import type { Action } from "../actions/Action";
import { Grid, GridRow, GridCol, Text, List } from "../components/BaseComponents";
import Persona from "../components/Persona";

type Props = {
  unit: UnitState,
  dispatch: (Action) => void,
  match: Object,
};

class UnitPage extends React.Component {
  props: Props;
  render() {
    const { unit } = this.props;
    return (
      <Grid>
        <GridRow>
          <GridCol lgWidth={2} backgroundColor="neutralTertiaryAlt">
            <List>
              <li>
                <Text weight="semibold" size="l">Areas</Text>
                <List>
                  {unit.data.areas.map((a, k) => (
                    <li key={k}><Text size="l">{a.get("name")}</Text></li>
                  ))}
                </List>
              </li>
              <li>
                <Text weight="semibold" size="l">Members</Text>
                  {unit.data.members.map((memberId, k) =>
                    <Persona id={memberId} key={k} />
                  )}
              </li>
            </List>
          </GridCol>
        </GridRow>
      </Grid>
    );
  }
}

const mapState = (state: RootState) => {
  return {
    unit: state.unit,
  };
};

export { UnitPage };
export default connect(mapState)(UnitPage);
