import {h} from 'preact';
import HTMLDiff from 'htmldiff-js';

import {Section} from '../../doc';

// TODO(ddc) style the diff
export const SectionBody = (props: {
  HeadSection: Section;
  BaseSection: Section;
}) => {
  if (props.BaseSection.Content === props.HeadSection.Content) {
    return (
      <section
			  className="prose"
        dangerouslySetInnerHTML={{
          __html: props.HeadSection.Content,
        }}
      />
    );
  }

  return (
    <section
			className="prose"
      dangerouslySetInnerHTML={{
        __html: HTMLDiff.execute(
          props.BaseSection.Content,
          props.HeadSection.Content,
        ),
      }}
    />
  );
};
