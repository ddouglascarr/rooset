// @flow
import React from "react";
import { Persona as FabricPersona, PersonaSize } from "office-ui-fabric-react";
import type { Uuid } from "../types";
import { fetchPublicUserData } from "../services/userService";

type Props = {
  id: Uuid,
  size: "tiny" | "extraSmall" | "small" | "regular" | "large" | "extraLarge",
};

type State = {|
  status: "LOADING" | "ERROR" | "READY",
  data?: {|
    username: String,
    displayName: String,
  |},
|};

export default class Persona extends React.Component {
  props: Props;
  state: State;

  constructor(props: Props, context: any) {
    super(props, context);
    this.state = { status: "LOADING" };

    fetchPublicUserData(props.id).then(
      m => m
        ? this.setState({
            status: "READY",
            data: {
              username: m.username,
              displayName: m.displayName,
            },
          })
        : this.setState({ status: "ERROR" }),
    );
  }

  render() {
    const { state } = this;
    const size = this.props.size || "regular";
    return (
      <FabricPersona
        size={PersonaSize[size]}
        primaryText={state.data ? state.data.username : "..."}
        style={{ marginTop: "2px", marginBottom: "2px" }}
      />
    );
  }
}
