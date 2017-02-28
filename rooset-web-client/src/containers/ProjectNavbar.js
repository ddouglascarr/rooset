// @flow

import React from "react";
import { CommandBar } from "office-ui-fabric-react/lib/CommandBar";

class ProjectNavbar extends React.Component {
  static contextTypes: { router: Object };

  render() {
    return (
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
    );
  }
}

ProjectNavbar.contextTypes = { router: React.PropTypes.object };

export default ProjectNavbar;
