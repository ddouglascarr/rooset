// @flow

import React from "react";
import type { Dispatch } from "redux";
import { connect } from "react-redux";
import { LinkButton } from "../components/BaseComponents";
import type { SessionState } from "../reducers/models/SessionState";

type Props = {
  location: any,
  session: SessionState,
  dispatch: Dispatch<*>,
};

class SessionContainer extends React.Component {
  props: Props;

  renderLoggedIn() {
    return <LinkButton to="/">Logout</LinkButton>;
  }

  renderLoggedOut() {
    return <LinkButton to="/login">Login</LinkButton>;
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

export { SessionContainer };
export default connect(mapState)(SessionContainer);
