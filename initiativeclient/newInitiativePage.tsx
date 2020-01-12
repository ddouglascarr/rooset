import {render, h, Component} from 'preact';
import {EditorView} from 'prosemirror-view';

import {messages} from 'messages';
import {initProsemirror} from 'editor';

type Props = {
  tk: string;
  areaID: number;
  proseMirrorEl: HTMLElement;
};

type State =
  | {status: 'IDLE'}
  | {status: 'LOADING_LIST'}
  | {status: 'LOADING_DOC'; blobList: Array<messages.IBlobRecord>}
  | {
      status: 'READY';
      doc: string;
    }
  | {status: 'COMPLETE'; commitTk: string}
  | {status: 'FAILED'; message: string};

class NewInitiativePage extends Component<Props, State> {
  editorView: null | EditorView;

  constructor() {
    super();
    this.state = {status: 'IDLE' as const};
    this.editorView = null;
  }

  loadDoc = async () => {
    const reqBody = messages.GetDocReq.fromObject({
      GitRef: 'master',
    });
    try {
      const resp = await window.fetch('http://localhost:8080/get-doc', {
        method: 'post',
        mode: 'cors',
        cache: 'no-cache',
        headers: {
          Authorization: this.props.tk,
        },
        body: JSON.stringify(messages.GetDocReq.toObject(reqBody)),
      });
      if (resp.ok) {
        const body = await resp.json();
        const reason = messages.GetDocReq.verify(body);
        if (reason) {
          this.setState({
            status: 'FAILED',
            message: `invalid server response ${reason}`,
          });
          return;
        }
        this.setState({
          status: 'READY',
          doc: messages.GetDocResp.fromObject(body).Blob!.Content!!,
        });
      } else {
        this.setState({
          status: 'FAILED',
          // TODO: parse this better
          message: 'Sorry, something went wrong with your network request.',
        });
      }
    } catch (err) {
      this.setState({
        status: 'FAILED',
        message:
          'Sorry, something went wrong with your network request. Please check your connection and try again',
      });
    }
  };

  createInitiative = async () => {
    if (this.editorView === null) return;
    const reqBody = new messages.NewInitiativeReq({
      Content: JSON.stringify(this.editorView.state.doc),
    });

    try {
      const resp = await window.fetch('http://localhost:8080/new-initiative', {
        method: 'post',
        mode: 'cors',
        cache: 'no-cache',
        headers: {
          Authorization: this.props.tk,
        },
        body: JSON.stringify(messages.NewInitiativeReq.toObject(reqBody)),
      });
      if (resp.ok) {
        const body = await resp.json();
        this.setState({
          status: 'COMPLETE',
          // TODO: any
          commitTk: await body.Tk,
        });
      } else {
        this.setState({
          status: 'FAILED',
          message: 'failed to create initiative',
        });
      }
    } catch (err) {
      this.setState({
        status: 'FAILED',
        message:
          'Sorry, something went wrong with your network request. Please check your connection and try again',
      });
    }
  };

  componentDidMount() {
    this.setState({status: 'LOADING_LIST'});
    this.loadDoc();
  }

  componentDidUpdate(prevProps: Props, prevState: State) {
    if (this.state.status === 'READY' && prevState.status !== 'READY') {
      this.editorView = initProsemirror(
        this.props.proseMirrorEl,
        JSON.parse(this.state.doc),
      );
    }

    if (this.state.status === 'COMPLETE') {
      window.location.href = `/initiative/new.html?area_id=${this.props.areaID}&tk=${this.state.commitTk}`;
    }
  }

  render() {
    if (this.state.status === 'READY') {
      return (
        <div>
          <button onClick={() => this.createInitiative()}>Next...</button>
        </div>
      );
    }
    return <div>Loading: {this.state.status}</div>;
  }
}

export const initNewInitiativePage = (args: {
  fileListEl: HTMLElement;
  proseMirrorEl: HTMLElement;
  areaID: number;
  tk: string;
}) => {
  render(
    <NewInitiativePage
      areaID={args.areaID}
      proseMirrorEl={args.proseMirrorEl}
      tk={args.tk}
    />,
    args.fileListEl,
  );
};
