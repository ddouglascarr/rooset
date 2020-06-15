import {h, Fragment} from 'preact';
import {useState} from 'preact/hooks';
import {Doc} from '../../doc';
import {
  SectionEditing,
  SectionEditable,
  SectionNotEditableIssueConflict,
} from '../Section';

type Props = {
  BaseDoc: Doc;
  HeadDoc: Doc;
  OnUpdateSection: (sectionID: string, content: string) => void;
  OpenAdmittedSections: {[SectionID: string]: {IssueID: number}};
};
type State = {
  EditingSectionID: string | null;
};

export const Sections = (props: Props) => {
  const [state, setState] = useState<State>({EditingSectionID: null});
  return (
    <Fragment>
      {props.HeadDoc.Order.map(ID => {
        if (state.EditingSectionID && ID === state.EditingSectionID) {
          return (
            <SectionEditing
              key={ID}
              HeadSection={props.HeadDoc.Sections[ID]}
              SectionID={ID}
              OnEditComplete={(sectionID: string, content: string) => {
                props.OnUpdateSection(sectionID, content);
                setState({EditingSectionID: null});
              }}
            />
          );
        }

        if (props.OpenAdmittedSections[ID]) {
          return (
            <SectionNotEditableIssueConflict
              key={ID}
              SectionID={ID}
              BaseSection={props.BaseDoc.Sections[ID]}
              ConflictingIssueID={props.OpenAdmittedSections[ID].IssueID}
              Dimmed={state.EditingSectionID !== null}
            />
          );
        }

        return (
          <SectionEditable
            key={ID}
            SectionID={ID}
            BaseSection={props.BaseDoc.Sections[ID]}
            HeadSection={props.HeadDoc.Sections[ID]}
            OnEdit={(id: string) => {
              setState({EditingSectionID: id});
            }}
            Dimmed={state.EditingSectionID !== null}
          />
        );
      })}
    </Fragment>
  );
};
