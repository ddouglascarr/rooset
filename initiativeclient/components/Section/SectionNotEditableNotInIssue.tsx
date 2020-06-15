import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';

type Props = {
  SectionID: string;
  BaseSection: Section;
};

export const SectionNotEditableNotInIssue = (props: Props) => (
  <div className="editor__section editor__section--dimmed">
    <div className="editor__section-tools" />
    <div className="editor__section-body rooset-document">
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.BaseSection}
      />
    </div>
  </div>
);
