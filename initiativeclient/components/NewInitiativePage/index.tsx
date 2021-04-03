import { h } from "preact";

import { compareDocs } from "../../doc";
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
} from "../../hooks";
import { Sections } from "./Sections";
import { Button, ControlBox } from "../UI";
import { Editor } from "../Editor";

export type NewInitiativePageProps = {
  csrf_secret: string;
  rev_tk: string;
  desc_tk: string;
  rev_base_sha: string;
  desc_base_sha: string;
  user_id: string;
  area_id: string;
  policy_id: string;
  issue_id: string | undefined;
  docsvc_host_external: string;
  open_admitted_sections: { [SectionID: string]: { IssueID: number } };
};

const RevViewComp = (
  props: NewInitiativePageProps & { pageState: RevView }
) => (
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
      baseDoc={props.pageState.data.rev.data.oldDoc}
      headDoc={props.pageState.data.rev.data.newDoc}
      onUpdateSection={(sectionID: string, content: string) => {
        props.pageState.data.rev.updateSection(sectionID, content);
      }}
      openAdmittedSections={props.open_admitted_sections}
      editingEnabled={false}
    />
  </div>
);

const RevEditComp = (
  props: NewInitiativePageProps & { pageState: RevEdit }
) => (
  <div>
    <ControlBox message="Make proposed changes to a section.">
      <Button
        onClick={() => {
          props.pageState.cancel();
        }}
      >
        Cancel
      </Button>

      <Button mode="primary" disabled>
        Next
      </Button>
    </ControlBox>
    <Sections
      baseDoc={props.pageState.data.rev.data.oldDoc}
      headDoc={props.pageState.data.rev.data.newDoc}
      onUpdateSection={(sectionID: string, content: string) => {
        props.pageState.data.rev.updateSection(sectionID, content);
      }}
      openAdmittedSections={props.open_admitted_sections}
      editingEnabled
    />
  </div>
);

const RevReadyComp = (
  props: NewInitiativePageProps & { pageState: RevReady }
) => (
  <div>
    <ControlBox message="Next, explain your proposal">
      <Button
        onClick={() => {
          props.pageState.editMore();
        }}
      >
        Edit Another Section
      </Button>
      <Button
        onClick={() => {
          props.pageState.data.rev.submitDocRev(
            props.docsvc_host_external,
            props.rev_tk
          );
        }}
        disabled={compareDocs(
          props.pageState.data.rev.data.oldDoc,
          props.pageState.data.rev.data.newDoc
        )}
        mode="primary"
      >
        Next
      </Button>
    </ControlBox>
    <Sections
      baseDoc={props.pageState.data.rev.data.oldDoc}
      headDoc={props.pageState.data.rev.data.newDoc}
      onUpdateSection={(sectionID: string, content: string) => {
        props.pageState.data.rev.updateSection(sectionID, content);
      }}
      openAdmittedSections={props.open_admitted_sections}
      editingEnabled={false}
    />
  </div>
);

const DescReadyComp = (
  props: NewInitiativePageProps & { pageState: DescReady }
) => (
  <div>
    <ControlBox message="Describe your proposal">
      <Button
        type="submit"
        mode="primary"
        onClick={() => {
          props.pageState.data.desc.submitDesc(
            props.docsvc_host_external,
            props.desc_tk
          );
        }}
      >
        Review
      </Button>
    </ControlBox>
    <div>
      <label className="rst-label" for="name">
        Title
      </label>
      <input
        className="rst-text-input"
        name="name"
        type="text"
        value={props.pageState.data.title}
        onChange={(e) => props.pageState.setTitle(e.currentTarget.value)}
      />

      <Editor
        initialValue={props.pageState.data.desc.data.descTemplate.content}
        onEditorChange={(content: string) => {
          props.pageState.data.desc.updateDescContent(content);
        }}
      />
    </div>
  </div>
);

const ReviewComp = (props: NewInitiativePageProps & { pageState: Review }) => (
  <form method="POST" action="/initiative/create">
    <ControlBox message="Review">
      <Button type="submit" mode="primary">
        Submit
      </Button>
    </ControlBox>
    <div>
      <h2>{props.pageState.data.title}</h2>
    </div>
    <input
      name="_webmcp_routing.default.module"
      type="hidden"
      value="initiative"
    />
    <input name="_webmcp_routing.default.mode" type="hidden" value="forward" />
    <input name="_webmcp_csrf_secret" type="hidden" value={props.csrf_secret} />
    <input name="_webmcp_routing.default.view" type="hidden" value="new" />
    <input name="area_id" type="hidden" value={props.area_id} />
    <input name="policy_id" type="hidden" value={props.policy_id} />
    <input
      name="rev_tk"
      type="hidden"
      value={props.pageState.data.rev.data.newDocTk}
    />
    <input
      name="desc_tk"
      type="hidden"
      value={props.pageState.data.desc.data.newDescTk}
    />
    <input name="name" type="hidden" value={props.pageState.data.title} />
  </form>
);

export const NewInitiativePage = (props: NewInitiativePageProps) => {
  const pageState = useInitiativePageState(
    props.docsvc_host_external,
    props.rev_tk,
    props.desc_tk,
    props.rev_base_sha,
    props.desc_base_sha
  );
  if (pageState instanceof Failed) {
    return <div>Fetching doc failed: {pageState.data.message}</div>;
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
    return <DescReadyComp pageState={pageState} {...props} />;
  }

  if (pageState instanceof Review) {
    return <ReviewComp pageState={pageState} {...props} />;
  }

  return <div>{pageState.constructor.name}</div>;
};
