import { h } from "preact";

// TOD: any
export const SectionContainer = (props: {
  dimmed?: boolean;
  children: any;
}) => (
  <div
    className={[
      "flex border-b pb-4 mb-8",
      props.dimmed ? "opacity-50" : "opacity-100",
    ].join(" ")}
  >
    {props.children}
  </div>
);

// TODO: any
export const Tools = (props: { children?: any; Grid?: boolean }) => (
  <div
    className={[
      "flex-grow-0 flex-shrink-0 w-56 pr-8",
      props.Grid ? "grid grid-cols-2" : "",
    ].join(" ")}
  >
    {props.children}
  </div>
);

export const SectionBodyContainer = (props: { children: any }) => (
  <div className="flex-grow">{props.children}</div>
);
