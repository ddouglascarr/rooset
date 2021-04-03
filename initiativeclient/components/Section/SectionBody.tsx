import { h } from "preact";
import HTMLDiff from "htmldiff-js";

import { Section } from "../../doc";

// TODO(ddc) style the diff
export const SectionBody = (props: {
  headSection: Section;
  baseSection: Section;
}) => {
  if (props.baseSection.content === props.headSection.content) {
    return (
      <section
        className="prose"
        dangerouslySetInnerHTML={{
          __html: props.headSection.content,
        }}
      />
    );
  }

  return (
    <section
      className="prose"
      dangerouslySetInnerHTML={{
        __html: HTMLDiff.execute(
          props.baseSection.content,
          props.headSection.content
        ),
      }}
    />
  );
};
