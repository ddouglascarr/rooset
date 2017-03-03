// @flow

import React from "react";
import type { Dispatch } from "redux";
import { connect } from "react-redux";
import { LinkButton } from "../components/BaseComponents";
import Persona from "../components/Persona";
import type { SessionState } from "../reducers/models/SessionState";
import { buildSessionEndAction } from "../actions/SessionAction";
import { ContextualMenu, ContextualMenuItemType } from "office-ui-fabric-react";

type Props = {
  location: any,
  session: SessionState,
  dispatch: Dispatch<*>,
};

type State = {
  isMenuVisible: boolean,
  target?: any,
};

class SessionContainer extends React.Component {
  props: Props;
  state: State;

  constructor() {
    super();
    this.state = { isMenuVisible: false };
  }

  renderMenu() {
    const { session, dispatch } = this.props;
    const { data } = session;
    if (!data) return null;
    return (
      <ContextualMenu
        items={[
          {
            key: "heading",
            name: "Signed in as",
            itemType: ContextualMenuItemType.Header,
          },
          { key: "displayName", name: data.displayName },
          { key: "divider-0", itemType: ContextualMenuItemType.Divider },
          {
            key: "log-out",
            name: "Sign out",
            onClick: () => {
              this.setState({ isMenuVisible: false });
              dispatch(buildSessionEndAction());
            }
          },
        ]}
        target={this.state.target}
        isBeakVisible={true}
      />
    );
  }

  renderLoggedIn() {
    const { data } = this.props.session;
    const { isMenuVisible } = this.state;
    return (
      <div>
        <Persona
          id={data ? data.id : ""}
          size="small"
          hidePersonaDetails={true}
          onClick={e => {
            this.setState({
              isMenuVisible: !isMenuVisible,
              target: e.target,
            });
          }}
        />
        {isMenuVisible ? this.renderMenu() : null}
      </div>
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
