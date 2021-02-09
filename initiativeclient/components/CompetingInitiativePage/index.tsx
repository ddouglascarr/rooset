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

export type Props = {
  Tk: string;
  BaseDocSHA: string;
  UserID: string;
  AreaID: string;
  IssueID: string;
  DocsvcHostExternal: string;
  IssueSections: {[SectionID: string]: {}};
};

export const CompetingInitiativePage = (props: Props) => {
  const docState = useDocState(
    props.DocsvcHostExternal,
    props.Tk,
    props.BaseDocSHA,
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
      window.location.href = `/initiative/new.html?area_id=${props.AreaID}&issue_id=${props.IssueID}&tk=${docState.Data.NewDocTk}`;
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
            )}>
            Propose Changes
          </Button>
        ) : (
          <button disabled>Submitting ...</button>
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
        IssueSections={props.IssueSections}
      />
    </div>
  );
};
