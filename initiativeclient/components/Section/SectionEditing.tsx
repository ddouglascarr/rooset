import { h } from "preact";
import { useState } from "preact/hooks";
import { Editor } from "../Editor";

import { Section } from "../../doc";
import { SectionContainer, Tools, SectionBodyContainer } from "./UI";
import { Button } from "../UI";

export type SectionEditingProps = {
  sectionID: string;
  headSection: Section;
  onEditComplete: (sectionID: string, content: string) => void;
};

export const SectionEditing = (props: SectionEditingProps) => {
  const [newContent, setNewContent] = useState<string>(
    props.headSection.content
  );
  return (
    <SectionContainer>
      <Tools Grid>
        <div></div>
        <div>
          <Button
            mode="primary"
            onClick={() => props.onEditComplete(props.sectionID, newContent)}
          >
            Done
          </Button>
        </div>
      </Tools>
      <SectionBodyContainer>
        <Editor
          initialValue={props.headSection.content}
          onEditorChange={(content: string) => {
            setNewContent(content);
          }}
        />
      </SectionBodyContainer>
    </SectionContainer>
  );
};
