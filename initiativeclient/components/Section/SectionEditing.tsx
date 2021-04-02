import {h} from 'preact';
import {useState} from 'preact/hooks';
import {Editor} from '../Editor';

import {Section} from '../../doc';
import {SectionContainer, Tools, SectionBodyContainer} from './UI';
import {Button} from '../UI';

export type SectionEditingProps = {
  SectionID: string;
  HeadSection: Section;
  OnEditComplete: (sectionID: string, content: string) => void;
};

export const SectionEditing = (props: SectionEditingProps) => {
  const [newContent, setNewContent] = useState<string>(props.HeadSection.Content);
  return (
    <SectionContainer>
      <Tools Grid>
        <div></div>
        <div>
          <Button 
            mode="primary"
            onClick={() => props.OnEditComplete(props.SectionID, newContent)}
          >
            Done
          </Button>
        </div>
      </Tools>
      <SectionBodyContainer>
        <Editor
          initialValue={props.HeadSection.Content}
          onEditorChange={(content: string) => { setNewContent(content) }}
        />
      </SectionBodyContainer>
    </SectionContainer>
  );
};

