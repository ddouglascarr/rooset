import { h } from "preact";
import { Section } from "../../doc";
import { SectionBody } from "./SectionBody";
import { SectionContainer, Tools, SectionBodyContainer } from "./UI";
import { Button } from "../UI";

export type SectionEditableProps = {
  sectionID: string;
  baseSection: Section;
  headSection: Section;
  onEdit: (sectionID: string) => void;
  dimmed: boolean;
  enabled: boolean;
};

export const SectionEditable = (props: SectionEditableProps) => (
  <SectionContainer dimmed={props.dimmed}>
    <Tools Grid>
      {props.enabled ? (
        <div>
          <Button onClick={() => props.onEdit(props.sectionID)}>Edit</Button>
        </div>
      ) : (
        <div></div>
      )}
      <div></div>
    </Tools>
    <SectionBodyContainer>
      <SectionBody
        baseSection={props.baseSection}
        headSection={props.headSection}
      />
    </SectionBodyContainer>
  </SectionContainer>
);
