// @flow

import React from "react";
import type { Dispatch } from "redux";
import { connect } from "react-redux";
import { LinkButton } from "../components/BaseComponents";
import Persona from "../components/Persona";
import type { SessionState } from "../reducers/models/SessionState";

type Props = {
  location: any,
  session: SessionState,
  dispatch: Dispatch<*>,
};

class SessionContainer extends React.Component {
  props: Props;

  renderLoggedIn() {
    const { session } = this.props;
    return (
      <Persona
        id={session.data ? session.data.id : ""}
        size="small"
        hidePersonaDetails={true}
      />
    );
  }

  renderLoggedOut() {
    const { location } = this.props;
    const to = {
      pathname: "/login",
      state: { from: location ? location.pathname : window.location.pathname },
    };
    return <LinkButton to={to}>Login</LinkButton>;
  }

  render() {
    const { session } = this.props;
    if (session.status === "LOGGED_IN") {
      return this.renderLoggedIn();
    }
    return this.renderLoggedOut();
  }
}

const mapState = state => {
  return { session: state.session };
};

// export { SessionContainer };
export default connect(mapState)(SessionContainer);
