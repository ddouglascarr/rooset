// @flow
// Flow #1964 dictates he janky props.children usage. If its fixed, refactor
import React from "react";


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
  rightAlign?: bool,
  backgroundColor?: String
};
export function GridCol(props: GridColProps) {
  const { children, smWidth, lgWidth, rightAlign } = props;
  let classNames: string = "ms-Grid-col";
  if (props.smWidth) {
    classNames += ` ms-u-sm${String(smWidth)}`;
  }
  if (props.lgWidth) {
    classNames += ` ms-u-xl${String(lgWidth)}`;
  }
  if (rightAlign) classNames += " ms-u-textAlignRight";
  return <div className={classNames}>{children ? children : null}</div>;
}

type TextProps = {
  children?: string,
  weight?: "light" | "semilight" | "regular" | "bold",
  size?: "s" | "m" | "l" | "xxl",
  inverse?: bool,
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
