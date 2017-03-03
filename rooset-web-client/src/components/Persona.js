// @flow
import React from "react";
import { Persona as FabricPersona, PersonaSize } from "office-ui-fabric-react";
import type { Uuid } from "../types";
import { fetchPublicUserData } from "../services/userService";

type Props = {
  id: Uuid,
  size: "tiny" | "extraSmall" | "small" | "regular" | "large" | "extraLarge",
  hidePersonaDetails?: boolean,
  onClick?: () => void,
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
    const { onClick, hidePersonaDetails } = this.props;
    const { state } = this;
    const size = this.props.size || "regular";
    return (
      <FabricPersona
        onClick={onClick}
        size={PersonaSize[size]}
        primaryText={state.data ? state.data.displayName : "..."}
        style={{ marginTop: "2px", marginBottom: "2px" }}
        hidePersonaDetails={hidePersonaDetails}
      />
    );
  }
}
