import {h, Component, Fragment, createRef, RefObject} from 'preact';
import HTMLDiff from 'htmldiff-js';

import {Section, Doc} from './doc';

// TODO: any
declare const tinymce: any;

const SectionBody = (props: {NewSection: Section; OldSection: Section}) => {
  if (props.OldSection.Content === props.NewSection.Content) {
    return (
      <section
        dangerouslySetInnerHTML={{
          __html: props.NewSection.Content,
        }}
      />
    );
  }

  return (
    <section
      dangerouslySetInnerHTML={{
        __html: HTMLDiff.execute(
          props.OldSection.Content,
          props.NewSection.Content,
        ),
      }}
    />
  );
};

type SectionEditorProps = {
  ID: string;
  OldSection: Section;
  NewSection: Section;
  EditingSectionID: null | string;
  ConflictingIssueID: null | number;
  onEdit: (sectionID: string) => void;
  onEditComplete: (sectionID: string, content: string) => void;
};
class SectionEditor extends Component<SectionEditorProps, {}> {
  private editor: any;
  private editorRef: RefObject<HTMLTextAreaElement>;

  constructor() {
    super();
    this.editorRef = createRef<HTMLTextAreaElement>();
  }

  initTinymce = () => {
    tinymce.init({
      target: this.editorRef.current,
      contextmenu: false,
      elementpath: false,
      branding: false,
      menubar: false,
      plugins: 'lists',
      toolbar: 'undo redo h3 bullist numlist',
      setup: (editor: any) => {
        this.editor = editor;
        editor.on('init', () => {
          editor.setContent(this.props.NewSection.Content);
        });
      },
    });
  };

  finishTinymce = () => {
    const newContent = this.editor.getContent();
    tinymce.remove(this.editor);
    this.props.onEditComplete(this.props.ID, newContent);
  };

  componentDidUpdate(prevProps: SectionEditorProps) {
    if (
      prevProps.EditingSectionID !== this.props.ID &&
      this.props.EditingSectionID === this.props.ID
    ) {
      this.initTinymce();
    }
  }

  render() {
    // this section is being edited
    if (this.props.EditingSectionID === this.props.ID) {
      return (
        <div className="pure-g">
          <div className="pure-u-1-5">
            <button
              className="pure-button pure-button-primary"
              onClick={() => this.finishTinymce()}>
              Done
            </button>
          </div>
          <div className="pure-u-4-5">
            <textarea ref={this.editorRef} />
          </div>
        </div>
      );
    }

    return (
      <div
        className={[
          'pure-g',
          this.props.EditingSectionID !== null ? 'editor__section--dimmed' : '',
        ].join(' ')}>
        <div className="pure-u-1-5 editor__section-tools">
          <button
            className="pure-button"
            onClick={() => this.props.onEdit(this.props.ID)}
            disabled={
              this.props.EditingSectionID !== null ||
              this.props.ConflictingIssueID !== null
            }>
            Edit
          </button>
          {this.props.ConflictingIssueID ? (
            <div>
              This section is under deliberation in issue{' '}
              <a href={`/issue/show/${this.props.ConflictingIssueID}.html`}>
                #{this.props.ConflictingIssueID}
              </a>
            </div>
          ) : null}
        </div>
        <div className="pure-u-4-5 rooset-document">
          <SectionBody
            OldSection={this.props.OldSection}
            NewSection={this.props.NewSection}
          />
        </div>
      </div>
    );

    // another section is being edited
    //
  }
}
type Props = {
  OldDoc: Doc;
  NewDoc: Doc;
  onUpdateDoc: (newDoc: Doc) => void;
  OpenAdmittedSections: {[sectionID: string]: {IssueID: number}};
};
type State = {EditingSectionID: null | string};
export class AreaEditor extends Component<Props, State> {
  constructor() {
    super();
    this.state = {EditingSectionID: null};
  }

  private updateSection = (doc: Doc, sectionID: string, content: string) => {
    return {
      ...doc,
      Sections: {
        ...doc.Sections,
        [sectionID]: {
          ID: sectionID,
          Content: content,
        },
      },
    };
  };

  render() {
    return (
      <Fragment>
        {this.props.NewDoc.Order.map(ID => (
          <SectionEditor
            key={ID}
            OldSection={this.props.OldDoc.Sections[ID]}
            NewSection={this.props.NewDoc.Sections[ID]}
            EditingSectionID={this.state.EditingSectionID}
            ID={ID}
            onEdit={(id: string) => {
              this.setState({EditingSectionID: id});
            }}
            onEditComplete={(sectionID: string, newContent: string) => {
              this.props.onUpdateDoc(
                this.updateSection(this.props.NewDoc, sectionID, newContent),
              );
              this.setState({EditingSectionID: null});
            }}
            ConflictingIssueID={
              this.props.OpenAdmittedSections[ID]?.IssueID || null
            }
          />
        ))}
      </Fragment>
    );
  }
}
