// @flow
// Flow #1964 dictates he janky props.children usage. If its fixed, refactor

import React from "react";
import "office-ui-fabric-react/dist/css/fabric.css";
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
};
export function GridCol(props: GridColProps) {
  const { children, smWidth, lgWidth } = props;
  let classNames: string = "ms-Grid-col";
  if (props.smWidth) {
    classNames += ` ms-u-sm${String(smWidth)}`;
  }
  if (props.lgWidth) {
    classNames += ` ms-u-xl${String(lgWidth)}`;
  }
  return <div className={classNames}>{children ? children : null}</div>;
}
