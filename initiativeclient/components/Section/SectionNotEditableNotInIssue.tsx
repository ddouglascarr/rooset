import { h } from "preact";
import { Section } from "../../doc";
import { SectionBody } from "./SectionBody";
import { SectionContainer, Tools, SectionBodyContainer } from "./UI";

type Props = {
  sectionID: string;
  baseSection: Section;
};

export const SectionNotEditableNotInIssue = (props: Props) => (
  <SectionContainer dimmed>
    <Tools />
    <SectionBodyContainer>
      <SectionBody
        baseSection={props.baseSection}
        headSection={props.baseSection}
      />
    </SectionBodyContainer>
  </SectionContainer>
);
