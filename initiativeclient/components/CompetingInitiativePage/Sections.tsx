import {h, Fragment} from 'preact';
import {useState} from 'preact/hooks';
import {Doc} from '../../doc';
import {
  SectionEditing,
  SectionEditable,
  SectionNotEditableNotInIssue,
} from '../Section';

type Props = {
  BaseDoc: Doc;
  HeadDoc: Doc;
  OnUpdateSection: (sectionID: string, content: string) => void;
  IssueSections: {[SectionID: string]: {}};
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

        if (!props.IssueSections[ID]) {
          return (
            <SectionNotEditableNotInIssue
              key={ID}
              SectionID={ID}
              BaseSection={props.BaseDoc.Sections[ID]}
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
