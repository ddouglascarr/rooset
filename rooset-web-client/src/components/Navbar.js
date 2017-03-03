// @flow

import React from "react";
import { Grid, GridRow, GridCol, Text } from "../components/BaseComponents";
import SessionContainer from "../containers/SessionContainer";

class ProjectNavbar extends React.Component {
  render() {
    return (
      <div
        style={{
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
          height: "50px",
        }}
        className="ms-bgColor-black ms-fontColor-neutralLight ms-font-m"
      >
        <Grid>
          <GridRow>
            <GridCol lgWidth={2}>
              <Text size="xxl" weight="semibold" inverse>Rooset</Text>
            </GridCol>
            <GridCol lgWidth={10}>
              <div
                className=""
                id="navbar-right-items"
                style={{
                  display: "flex",
                  flexDirection: "column",
                  alignItems: "flex-end",
                }}
              >
                <SessionContainer />
              </div>
            </GridCol>
          </GridRow>
        </Grid>
      </div>
    );
  }
}

export default ProjectNavbar;
