// @flow

import React from "react";
import { Link } from "react-router-dom";
import { Container } from "../components/BaseComponents";
import { CommandBar } from "office-ui-fabric-react/lib/CommandBar";

class ProjectNavbar extends React.Component {
  static contextTypes: { router: React.PropTypes.object };

  render() {
    return (
      <Container>
        <CommandBar
          isSearchBoxVisible={false}
          items={[
            {
              name: "Rooset",
              onClick: () => this.context.router.push("/"),
            },
            {
              name: "Login",
              onClick: () => this.context.router.push("/login"),
            },
          ]}
        />
      </Container>
    );
  }
}

ProjectNavbar.contextTypes = { router: React.PropTypes.object };

export default ProjectNavbar;
