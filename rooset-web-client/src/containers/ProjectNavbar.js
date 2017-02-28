// @flow

import React from "react";
import { Grid, GridRow, GridCol, Text } from "../components/BaseComponents";

import "../components/style/project.css";

class ProjectNavbar extends React.Component {
  static contextTypes: { router: Object };

  render() {
    return (
      <div className="project-header ms-bgColor-black ms-fontColor-neutralLight ms-font-m">
        <Grid>
          <GridRow>
              <GridCol lgWidth={2} rightAlign>
                <Text size="xxl" weight="bold" inverse>Rooset</Text>
              </GridCol>
          </GridRow>
        </Grid>
      </div>
    );
  }
}

ProjectNavbar.contextTypes = { router: React.PropTypes.object };

export default ProjectNavbar;
