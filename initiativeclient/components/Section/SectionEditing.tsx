import {h, Component, createRef, RefObject} from 'preact';
import {Section} from '../../doc';
import {SectionContainer, Tools, SectionBodyContainer} from './UI';
import {Button} from '../UI'

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
			<SectionContainer>
        <Tools Grid>
					<div></div>
					<div>
            <Button mode="primary" onClick={() => this.finishTinymce()}>
              Done
            </Button>
					</div>
				</Tools>
        <SectionBodyContainer>
          <textarea ref={this.editorRef} />
        </SectionBodyContainer>
			</SectionContainer>
    );
  }
}
