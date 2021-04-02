import {h} from 'preact';
import {Section} from '../../doc';
import {SectionBody} from './SectionBody';
import {SectionContainer, Tools, SectionBodyContainer} from './UI';
import {Button} from '../UI'

export type SectionEditableProps = {
  SectionID: string;
  BaseSection: Section;
  HeadSection: Section;
  OnEdit: (sectionID: string) => void;
  Dimmed: boolean;
  Enabled: boolean;
};

export const SectionEditable = (props: SectionEditableProps) => (
  <SectionContainer
		Dimmed={props.Dimmed}
	>
    <Tools Grid>
      {props.Enabled ? (
	  <div>
		<Button onClick={() => props.OnEdit(props.SectionID)}>
          Edit
        </Button>
	  </div>	
      ) : (<div></div>)}
	  <div></div>
    </Tools>
    <SectionBodyContainer>
      <SectionBody
        BaseSection={props.BaseSection}
        HeadSection={props.HeadSection}
      />
    </SectionBodyContainer>
  </SectionContainer>
);

