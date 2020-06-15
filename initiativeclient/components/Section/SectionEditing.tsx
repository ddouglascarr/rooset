import {h, Component, createRef, RefObject} from 'preact';
import {Section} from '../../doc';

// TODO: any
declare const tinymce: any;

export type SectionEditingProps = {
  SectionID: string;
  HeadSection: Section;
  OnEditComplete: (sectionID: string, content: string) => void;
};

export class SectionEditing extends Component<SectionEditingProps, {}> {
  private editor: any;
  private editorRef: RefObject<HTMLTextAreaElement>;

  constructor() {
    super();
    this.editorRef = createRef<HTMLTextAreaElement>();
  }

  initTinymce = () => {
    console.log('init tinymce');
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
          editor.setContent(this.props.HeadSection.Content);
        });
      },
    });
  };

  finishTinymce = () => {
    const newContent = this.editor.getContent();
    tinymce.remove(this.editor);
    this.props.OnEditComplete(this.props.SectionID, newContent);
  };

  componentDidMount() {
    this.initTinymce();
  }

  render() {
    return (
      <div className="editor__section">
        <div className="editor__section-tools">
          <button className="" onClick={() => this.finishTinymce()}>
            Done
          </button>
        </div>
        <div className="editor__section-body">
          <textarea ref={this.editorRef} />
        </div>
      </div>
    );
  }
}
