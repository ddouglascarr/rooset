import { h, Fragment } from "preact";
import { useState } from "preact/hooks";
import { Doc } from "../../doc";
import {
  SectionEditing,
  SectionEditable,
  SectionNotEditableNotInIssue,
} from "../Section";

type Props = {
  baseDoc: Doc;
  headDoc: Doc;
  onUpdateSection: (sectionID: string, content: string) => void;
  issueSections: { [SectionID: string]: {} };
  editingEnabled: boolean;
};

type State = {
  editingSectionID: string | null;
};

export const Sections = (props: Props) => {
  const [state, setState] = useState<State>({ editingSectionID: null });
  return (
    <Fragment>
      {props.headDoc.order.map((id) => {
        if (state.editingSectionID && id === state.editingSectionID) {
          return (
            <SectionEditing
              key={id}
              headSection={props.headDoc.sections[id]}
              sectionID={id}
              onEditComplete={(sectionID: string, content: string) => {
                props.onUpdateSection(sectionID, content);
                setState({ editingSectionID: null });
              }}
            />
          );
        }

        if (!props.issueSections[id]) {
          return (
            <SectionNotEditableNotInIssue
              key={id}
              sectionID={id}
              baseSection={props.baseDoc.sections[id]}
            />
          );
        }

        return (
          <SectionEditable
            key={id}
            sectionID={id}
            baseSection={props.baseDoc.sections[id]}
            headSection={props.headDoc.sections[id]}
            onEdit={(id: string) => {
              setState({ editingSectionID: id });
            }}
            dimmed={state.editingSectionID !== null}
            enabled={props.editingEnabled}
          />
        );
      })}
    </Fragment>
  );
};
