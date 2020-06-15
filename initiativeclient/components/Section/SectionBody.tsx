import {h} from 'preact';
import HTMLDiff from 'htmldiff-js';

import {Section} from '../../doc';

export const SectionBody = (props: {
  HeadSection: Section;
  BaseSection: Section;
}) => {
  if (props.BaseSection.Content === props.HeadSection.Content) {
    return (
      <section
        dangerouslySetInnerHTML={{
          __html: props.HeadSection.Content,
        }}
      />
    );
  }

  return (
    <section
      dangerouslySetInnerHTML={{
        __html: HTMLDiff.execute(
          props.BaseSection.Content,
          props.HeadSection.Content,
        ),
      }}
    />
  );
};
