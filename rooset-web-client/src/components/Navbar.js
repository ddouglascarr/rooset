// @flow

import React from "react";
import { Grid, GridRow, GridCol, Text  } from "../components/BaseComponents";
import SessionContainer from "../containers/SessionContainer";

import "../components/style/project.css";

class ProjectNavbar extends React.Component {

  render() {
    return (
      <div className="project-header ms-bgColor-black ms-fontColor-neutralLight ms-font-m">
        <Grid>
          <GridRow>
              <GridCol lgWidth={2} >
                <Text size="xxl" weight="semibold" inverse>Rooset</Text>
              </GridCol>
                <div style={{ textAlign: "right" }}><SessionContainer /></div>
          </GridRow>
        </Grid>
      </div>
    );
  }
}

export default ProjectNavbar;
