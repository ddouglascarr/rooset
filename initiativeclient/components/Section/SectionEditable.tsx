import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';

export type SectionEditableProps = {
  SectionID: string;
  BaseSection: Section;
  HeadSection: Section;
  OnEdit: (sectionID: string) => void;
  Dimmed: boolean;
};

export const SectionEditable = (props: SectionEditableProps) => (
  <div
    className={[
      'editor__section',
      props.Dimmed ? 'editor__section--dimmed' : '',
    ].join(' ')}>
    <div className="editor__section-tools">
      <button className="" onClick={() => props.OnEdit(props.SectionID)}>
        Edit
      </button>
    </div>
    <div className="editor__section-body rooset-document">
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.HeadSection}
      />
    </div>
  </div>
);
