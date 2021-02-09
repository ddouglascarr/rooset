import {h} from 'preact';
import {useEffect} from 'preact/hooks';

import {
  useDocState,
  DocStateLoading,
  DocStateFailed,
  DocStateIdle,
  DocStateReady,
  DocStateComplete,
  compareDocs,
} from '../../doc';
import {Sections} from './Sections';
import {Button} from '../UI'

export type NewInitiativePageProps = {
  Tk: string;
  DocSHA: string;
  UserID: string;
  AreaID: string;
  IssueID: string | undefined;
  DocsvcHostExternal: string;
  OpenAdmittedSections: {[SectionID: string]: {IssueID: number}};
};

export const NewInitiativePage = (props: NewInitiativePageProps) => {
  const docState = useDocState(
    props.DocsvcHostExternal,
    props.Tk,
    props.DocSHA,
  );
  if (docState instanceof DocStateFailed) {
    return <div>Fetching doc failed: {docState.Data.Message}</div>;
  }
  if (docState instanceof DocStateLoading || docState instanceof DocStateIdle) {
    return <div>Loading...</div>;
  }

  // navigate to new initiative page on DocStateComplete
  useEffect(() => {
    if (docState instanceof DocStateComplete) {
      const issueIDParam = props.IssueID ? `issue_id=${props.IssueID}&` : '';
      window.location.href = `/initiative/new.html?area_id=${props.AreaID}&${issueIDParam}tk=${docState.Data.NewDocTk}`;
    }
  }, [docState]);

  // Ready, Submitting, Complete states
  return (
    <div>
      <div className="flex justify-end pb-2">
        {docState instanceof DocStateReady ? (
          <Button
            onClick={() => {
              docState.submitDoc(
                props.UserID,
                props.DocsvcHostExternal,
                props.Tk,
              );
            }}
            disabled={compareDocs(
              docState.Data.OldDoc,
              docState.Data.NewDoc,
            )}
						mode="primary"
					>
            Propose Changes
          </Button>
        ) : (
          <Button disabled>Submitting ...</Button>
        )}
      </div>
      <Sections
        BaseDoc={docState.Data.OldDoc}
        HeadDoc={docState.Data.NewDoc}
        OnUpdateSection={(sectionID: string, content: string) => {
          docState instanceof DocStateReady
            ? docState.updateSection(sectionID, content)
            : null;
        }}
        OpenAdmittedSections={props.OpenAdmittedSections}
      />
    </div>
  );
};
