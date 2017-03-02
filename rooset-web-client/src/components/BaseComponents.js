// @flow
// Flow #1964 dictates he janky props.children usage. If its fixed, refactor
import React from "react";
import { connect } from "react-redux";
import {
  TextField as FabricTextField,
  Button as FabricButton,
} from "office-ui-fabric-react";

type ContainerProps = {
  children?: React.Children,
};
export function Container({ children }: ContainerProps) {
  return <div className="container">{children ? children : null}</div>;
}

type GridProps = {
  children?: React.Children,
};
export const Grid = ({ children }: GridProps) => (
  <div className="ms-Grid">{children ? children : null}</div>
);

type GridRowProps = { children?: React.Element<*> };
export const GridRow = ({ children }: GridRowProps) => (
  <div className="ms-Grid-row">{children ? children : null}</div>
);

type GridColProps = {
  children?: React.Children,
  smWidth?: number,
  lgWidth?: number,
  rightAlign?: boolean,
  backgroundColor?:
    | "neutralTertiary"
    | "neutralTertiaryAlt"
    | "neutralLight"
    | "neutralLighter"
    | "white",
};
export function GridCol(props: GridColProps) {
  const { children, smWidth, lgWidth, rightAlign, backgroundColor } = props;
  let classNames: string = "ms-Grid-col";
  if (props.smWidth) {
    classNames += ` ms-u-sm${String(smWidth)}`;
  }
  if (props.lgWidth) {
    classNames += ` ms-u-xl${String(lgWidth)}`;
  }
  if (rightAlign) classNames += " ms-u-textAlignRight";
  if (backgroundColor) classNames += ` ms-bgColor-${backgroundColor}`;
  return <div className={classNames}>{children ? children : null}</div>;
}

type TextProps = {
  children?: string,
  weight?: "light" | "semilight" | "regular" | "semibold",
  size?: "s" | "m" | "l" | "xxl",
  inverse?: boolean,
};
export function Text(props: TextProps) {
  const { children, inverse } = props;
  const weight = props.weight || "regular";
  const size = props.size || "m";
  let classNames: string = `ms-fontSize-${size}`;
  classNames += ` ms-fontWeight-${weight}`;
  if (inverse) classNames += ` ms-fontColor-neutralLighter`;
  return <span className={classNames}>{children ? children : ""}</span>;
}

type ListItemProps = {
  children?: React.Element<*>,
};
export function ListItem(props: ListItemProps) {
  const { children } = props;
  return <li>{children ? children : null}</li>;
}

type ListProps = {
  children?: React.Element<ListItem>,
};
export function List(props: ListProps) {
  const { children } = props;
  return (
    <ul style={{ listStyle: "none", paddingLeft: "1em" }}>
      {children ? children : null}
    </ul>
  );
}

type TextFieldProps = {
  label: string,
  onChange: () => void,
  value: string,
};
export function TextField(props: TextFieldProps) {
  const { label, onChange, value } = props;
  return <FabricTextField label={label} value={value} onChanged={onChange} />;
}

type ActionButtonProps = {
  onClick: () => void,
  children?: React.Children,
};
export class ActionButton extends React.Component {
  props: ActionButtonProps;

  render() {
    const { children, onClick } = this.props;
    return <FabricButton onClick={onClick}>{children}</FabricButton>;
  }
}

type LinkButtonProps = {
  context: any,
  to: string,
  children?: React.Children,
};
class _LinkButton extends React.Component {
  props: LinkButtonProps;

  render() {
    const { children, to } = this.props;
    return <FabricButton href={to}>{children}</FabricButton>
  }
}
export const LinkButton = connect(state => {
  return {};
})(_LinkButton);
