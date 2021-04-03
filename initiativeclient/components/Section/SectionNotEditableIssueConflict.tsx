import { h } from "preact";
import { Section } from "../../doc";
import { SectionBody } from "./SectionBody";
import { SectionContainer, Tools, SectionBodyContainer } from "./UI";

export type SectionNotEditableIssueConflictProps = {
  sectionID: string;
  baseSection: Section;
  dimmed: boolean;
  conflictingIssueID: number;
};

export const SectionNotEditableIssueConflict = (
  props: SectionNotEditableIssueConflictProps
) => (
  <SectionContainer dimmed={props.dimmed}>
    <Tools>
      <div className="text-xs italic">
        This section is under deliberation in issue{" "}
        <a
          className="underline text-blue-700"
          href={`/issue/show/${props.conflictingIssueID}.html`}
        >
          #{props.conflictingIssueID}
        </a>
      </div>
    </Tools>
    <SectionBodyContainer>
      <SectionBody
        baseSection={props.baseSection}
        headSection={props.baseSection}
      />
    </SectionBodyContainer>
  </SectionContainer>
);
