import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';

export type SectionNotEditableIssueConflictProps = {
  SectionID: string;
  BaseSection: Section;
  Dimmed: boolean;
  ConflictingIssueID: number;
};

export const SectionNotEditableIssueConflict = (
  props: SectionNotEditableIssueConflictProps,
) => (
  <div
    className={[
      'editor__section',
      props.Dimmed ? 'editor__section--dimmed' : '',
    ].join(' ')}>
    <div className="editor__section-tools">
      <div className="editor__section-tools-conflicting-section">
        This section is under deliberation in issue{' '}
        <a href={`/issue/show/${props.ConflictingIssueID}.html`}>
          #{props.ConflictingIssueID}
        </a>
      </div>
    </div>
    <div className="editor__section-body rooset-document">
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.BaseSection}
      />
    </div>
  </div>
);
