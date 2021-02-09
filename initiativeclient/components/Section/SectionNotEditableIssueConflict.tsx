import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';
import {SectionContainer, Tools, SectionBodyContainer} from './UI';
import {Button} from '../UI';

export type SectionNotEditableIssueConflictProps = {
  SectionID: string;
  BaseSection: Section;
  Dimmed: boolean;
  ConflictingIssueID: number;
};

export const SectionNotEditableIssueConflict = (
  props: SectionNotEditableIssueConflictProps,
) => (
	<SectionContainer Dimmed={props.Dimmed}>
    <Tools>
      <div className="text-xs italic">
        This section is under deliberation in issue{' '}
        <a className="underline text-blue-700" href={`/issue/show/${props.ConflictingIssueID}.html`}>
          #{props.ConflictingIssueID}
        </a>
      </div>
		</Tools>
		<SectionBodyContainer>
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.BaseSection}
      />
		</SectionBodyContainer>
  </SectionContainer>
);
