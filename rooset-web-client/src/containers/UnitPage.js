// @flow

import React from "react";
import { connect } from "react-redux";
import { UnitState } from "../reducers/models/UnitState";
import { RootState } from "../reducers/models/RootState";
import type { Action } from "../actions/Action";
import {
  Grid,
  GridRow,
  GridCol,
  Text,
  List,
  ListItem,
} from "../components/BaseComponents";
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
          <GridCol lgWidth={2} backgroundColor="neutralLight">
            <List>
              <ListItem>
                <Text weight="semibold" size="l">Areas</Text>
                <List>
                  {unit.data.areas.map((a, k) => (
                    <ListItem key={k}><Text size="l">{a.get("name")}</Text></ListItem>
                  ))}
                </List>
              </ListItem>
              <ListItem>
                <Text weight="semibold" size="l">Members</Text>
                <List>
                  {unit.data.members.map((memberId, k) => (
                    <ListItem key={k}>
                      <Persona id={memberId} key={k} size="small" />
                    </ListItem>
                  ))}
                </List>
              </ListItem>
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
