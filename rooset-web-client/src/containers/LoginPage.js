// @flow

import React from "react";
import type { Dispatch } from "redux";
import { connect } from "react-redux";
import { Redirect } from "react-router-dom";
import {
  Grid,
  GridRow,
  GridCol,
  Text,
  TextField,
  ActionButton,
} from "../components/BaseComponents";
import { buildLoginRequestAction } from "../actions/SessionAction";
import type { SessionState } from "../reducers/models/SessionState";

type LoginPageState = {
  username: string,
  password: string,
};

type LoginPageProps = {
  dispatch: Dispatch<*>,
  session: SessionState,
  location: any,
};

class LoginPage extends React.Component {
  state: LoginPageState;
  props: LoginPageProps;

  constructor() {
    super();
    this.state = { username: "", password: "" };
  }

  login() {
    const { dispatch } = this.props;
    const { username, password } = this.state;
    dispatch(buildLoginRequestAction({username, password}));
  }

  renderRedirect() {
    const { from } = this.props.location.state || { from: { pathname: '/' } };
    return <Redirect to={from} />
  }

  render() {
    const { username, password } = this.state;
    const { session } = this.props;
    if (session.status === "LOGGED_IN") return this.renderRedirect();
    return (
      <Grid>
        <GridRow>
          <GridCol lgWidth={2} backgroundColor="neutralLight" />
          <GridCol lgWidth={10} backgroundColor="white">
            <TextField
              label="Username"
              value={username}
              onChange={v => this.setState({ username: v })}
            />
            <TextField
              label="Password"
              value={password}
              onChange={v => this.setState({ password: v })}
            />
            <ActionButton onClick={() => this.login()}>
              Login
            </ActionButton>
          </GridCol>

        </GridRow>
      </Grid>
    );
  }
}

const mapState = state => {
  return {
    session: state.session,
  };
};

export { LoginPage };
export default connect(mapState)(LoginPage);
