import {h} from 'preact';

import {
  compareDocs,
} from '../../doc';
import {
  useInitiativePageState,
  Idle,
  Failed,
  Loading,
  RevView,
  RevEdit,
  RevReady,
  DescReady,
  Review,
} from '../../hooks';
import {Sections} from './Sections';
import {Button, ControlBox} from '../UI'
import { Editor } from '../Editor';

export type NewInitiativePageProps = {
  CSRFSecret: string;
  RevTk: string;
  DescTk: string;
  RevBaseSHA: string;
  DescBaseSHA: string;
  UserID: string;
  AreaID: string;
  PolicyID: string;
  IssueID: string | undefined;
  DocsvcHostExternal: string;
  OpenAdmittedSections: {[SectionID: string]: {IssueID: number}};
};

const RevViewComp = (props: NewInitiativePageProps & { pageState: RevView }) => (
  <div>
    <ControlBox message="">
        <Button
          onClick={() => {
            props.pageState.startEdit();
          }}
          mode="primary"
        >
          Create Proposal 
        </Button>
    </ControlBox>
    <Sections
      BaseDoc={props.pageState.Data.Rev.Data.OldDoc}
      HeadDoc={props.pageState.Data.Rev.Data.NewDoc}
      OnUpdateSection={(sectionID: string, content: string) => {
        props.pageState.Data.Rev.updateSection(sectionID, content)
      }}
      OpenAdmittedSections={props.OpenAdmittedSections}
      EditingEnabled={false}
    />
  </div>
);

const RevEditComp = (props: NewInitiativePageProps & { pageState: RevEdit }) => (
  <div>
    <ControlBox message="Make proposed changes to a section.">
        <Button
          onClick={() => { props.pageState.cancel() }}
        >
          Cancel
        </Button>

        <Button
          mode="primary"
          disabled
        >
          Next
        </Button>
    </ControlBox>
    <Sections
      BaseDoc={props.pageState.Data.Rev.Data.OldDoc}
      HeadDoc={props.pageState.Data.Rev.Data.NewDoc}
      OnUpdateSection={(sectionID: string, content: string) => {
        props.pageState.Data.Rev.updateSection(sectionID, content)
      }}
      OpenAdmittedSections={props.OpenAdmittedSections}
      EditingEnabled
    />
  </div>
);

const RevReadyComp = (props: NewInitiativePageProps & { pageState: RevReady }) => (
  <div>
    <ControlBox message="Next, explain your proposal">
        <Button 
          onClick={() => { props.pageState.editMore() }}
        >
          Edit Another Section
        </Button>
        <Button
          onClick={() => {
            props.pageState.Data.Rev.submitDocRev(
              props.DocsvcHostExternal,
              props.RevTk,
            );
          }}
          disabled={compareDocs(
            props.pageState.Data.Rev.Data.OldDoc,
            props.pageState.Data.Rev.Data.NewDoc,
          )}
          mode="primary"
        >
          Next
        </Button>
    </ControlBox>
    <Sections
      BaseDoc={props.pageState.Data.Rev.Data.OldDoc}
      HeadDoc={props.pageState.Data.Rev.Data.NewDoc}
      OnUpdateSection={(sectionID: string, content: string) => {
        props.pageState.Data.Rev.updateSection(sectionID, content)
      }}
      OpenAdmittedSections={props.OpenAdmittedSections}
      EditingEnabled={false}
    />
  </div>
);

const DescReadyComp = (props: NewInitiativePageProps & { pageState: DescReady }) => (
  <div>
    <ControlBox message="Describe your proposal">
      <Button type="submit" mode="primary" onClick={() => { props.pageState.Data.Desc.submitDesc(props.DocsvcHostExternal, props.DescTk) }} >Review</Button>
    </ControlBox>
    <div>
      <label className="rst-label" for="name">Title</label>
      <input 
        className="rst-text-input" 
        name="name"
        type="text" 
        value={props.pageState.Data.Title} 
        onChange={(e) => props.pageState.setTitle(e.currentTarget.value)}   
      />

      <Editor
        initialValue={props.pageState.Data.Desc.Data.DescTemplate.Content}
        onEditorChange={(content: string) => {
          props.pageState.Data.Desc.updateDescContent(content);
        }}
      />
    </div>
  </div>
);

const ReviewComp = (props: NewInitiativePageProps & { pageState: Review }) => (
  <form method="POST" action="/initiative/create">
    <ControlBox message="Review">
      <Button type="submit" mode="primary">Submit</Button>
    </ControlBox>
    <div>
      <h2>{props.pageState.Data.Title}</h2>
    </div>
    <input name="_webmcp_routing.default.module" type="hidden" value="initiative" />
    <input name="_webmcp_routing.default.mode" type="hidden" value="forward" />
    <input name="_webmcp_csrf_secret" type="hidden" value={props.CSRFSecret} />
    <input name="_webmcp_routing.default.view" type="hidden" value="new" />
    <input name="area_id" type="hidden" value={props.AreaID} />
    <input name="policy_id" type="hidden" value={props.PolicyID} />
    <input name="rev_tk" type="hidden" value={props.pageState.Data.Rev.Data.NewDocTk} />
    <input name="desc_tk" type="hidden" value={props.pageState.Data.Desc.Data.NewDescTk} />
    <input name="name" type="hidden" value={props.pageState.Data.Title} />
  </form>
);

export const NewInitiativePage = (props: NewInitiativePageProps) => {
  const pageState = useInitiativePageState(
    props.DocsvcHostExternal,
    props.RevTk,
    props.DescTk,
    props.RevBaseSHA,
    props.DescBaseSHA,
  );
  if (pageState instanceof Failed) {
    return <div>Fetching doc failed: {pageState.Data.Message}</div>;
  }
  if (pageState instanceof Idle || pageState instanceof Loading) {
    return <div>Loading...</div>;
  }

  if (pageState instanceof RevView) {
    return <RevViewComp pageState={pageState} {...props} />;
  }

  if (pageState instanceof RevEdit) {
    return <RevEditComp pageState={pageState} {...props} />;
  }

  if (pageState instanceof RevReady) {
    return <RevReadyComp pageState={pageState} {...props} />;
  }

  if (pageState instanceof DescReady) {
    return <DescReadyComp pageState={pageState} {...props} />
  }

  if (pageState instanceof Review) {
    return <ReviewComp pageState={pageState} {...props} />
  }

  return (<div>{pageState.constructor.name}</div>);
};
