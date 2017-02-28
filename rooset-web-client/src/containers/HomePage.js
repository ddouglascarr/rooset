import React from "react";
import { Grid, GridRow, GridCol } from "../components/BaseComponents";
import { Button } from "office-ui-fabric-react";

export default class HomePage extends React.Component {
  render() {
    return (
      <Grid>
        <GridRow>
          <GridCol lgWidth={3} />
          <GridCol lgWidth={9}>
            <Button>Foobar</Button>
          </GridCol>
        </GridRow>
      </Grid>
    );
  }
}
