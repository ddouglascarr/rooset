// @flow
import React from "react";
import { Persona as FabricPersona } from "office-ui-fabric-react";
import type { Uuid } from "../types";
import { fetchPublicUserData } from "../services/userService";

type Props = {
  id: Uuid,
};

type State = {|
  status: "LOADING" | "ERROR" | "READY",
  data?: {|
    firstName: String,
    lastName: String,
    username: String,
  |},
|};

export default class Persona extends React.Component {
  props: Props;
  state: State;

  constructor(props: Props, context: any) {
    super(props, context);
    this.state = { status: "LOADING" };
    fetchPublicUserData(props.id).then(m => m ? this.setState({
      status: "READY",
      data: {
        firstName: m.firstName,
        lastName: m.lastName,
        username: m.username,
      },
    }) : this.setState({status: "ERROR"}));
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
      return <FabricPersona primaryText={state.data ? state.data.username : "..."} />;
    }
    return null;
  }
}
