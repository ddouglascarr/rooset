import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';
import {SectionContainer, Tools, SectionBodyContainer} from './UI';

type Props = {
  SectionID: string;
  BaseSection: Section;
};

export const SectionNotEditableNotInIssue = (props: Props) => (
	<SectionContainer Dimmed>
		<Tools />
		<SectionBodyContainer>
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.BaseSection}
      />
		</SectionBodyContainer>
  </SectionContainer>
);
