// @flow
import React from "react";
import { Persona as FabricPersona } from "office-ui-fabric-react";
import type { Uuid } from "../types";
import { fetchPublicUserData } from "../services/userService";

type Props = {
  id: Uuid,
};

type State =
  | { status: "LOADING" }
  | { status: "ERROR" }
  | { status: "READY", firstName: String, lastName: String, username: String };

export default class Persona extends React.Component {
  props: Props;
  state: State;

  constructor(props: Props, context: any) {
    super(props, context);
    this.state = { status: "LOADING" };
    fetchPublicUserData(props.id).then(m =>
      this.setState({
        status: "READY",
        firstName: m.firstName,
        lastName: m.lastName,
        username: m.username,
      }));
  }

  renderLoading() {
    const { state } = this;
    if (state.status === "LOADING" || state.status === "ERROR") {
      return <FabricPersona primaryText="..." />;
    }
    return null;
  }

  render() {
    const { state } = this;
    if (state.status === "LOADING" || state.status === "ERROR") {
      return this.renderLoading();
    }
    if (state.status === "READY") {
      return <FabricPersona primaryText={state.username} />;
    }
    return null;
  }
}
