import {render, h, Component} from 'preact';

import {messages} from 'messages';

import {parseDoc, serializeDoc, Doc} from './doc';
import {AreaEditor} from './AreaEditor';

type Props = {
  Tk: string;
  DocSHA: string;
  UserID: string;
  AreaID: string;
  IssueID: string | undefined;
  DocsvcHostExternal: string;
};

type State =
  | {Status: 'IDLE'}
  | {Status: 'LOADING'}
  | {
      Status: 'READY';
      OldDoc: Doc;
      NewDoc: Doc;
    }
  | {Status: 'COMPLETE'; NewDocTk: string; NewDocSHA: string}
  | {Status: 'FAILED'; message: string};

class NewInitiativePage extends Component<Props, State> {
  constructor() {
    super();
    this.state = {Status: 'IDLE' as const};
  }

  loadDoc = async () => {
    const reqBody = messages.GetDocReq.fromObject({
      SHA: this.props.DocSHA,
    });
    try {
      const resp = await window.fetch(
        `${this.props.DocsvcHostExternal}/rpc/messages.GetDocReq`,
        {
          method: 'post',
          mode: 'cors',
          cache: 'no-cache',
          headers: {
            Authorization: this.props.Tk,
          },
          body: JSON.stringify(messages.GetDocReq.toObject(reqBody)),
        },
      );
      if (resp.ok) {
        const body = await resp.json();
        const reason = messages.GetDocReq.verify(body);
        if (reason) {
          this.setState({
            Status: 'FAILED',
            message: `invalid server response ${reason}`,
          });
          return;
        }
        const docStr = messages.GetDocResp.fromObject(body)?.Content;
        const doc = parseDoc(docStr);
        this.setState({
          Status: 'READY',
          OldDoc: doc,
          NewDoc: doc,
        });
      } else {
        this.setState({
          Status: 'FAILED',
          // TODO: parse this better
          message: 'Sorry, something went wrong with your network request.',
        });
      }
    } catch (err) {
      console.error(err);
      this.setState({
        Status: 'FAILED',
        message:
          'Sorry, something went wrong with your network request. Please check your connection and try again',
      });
    }
  };

  createDoc = async () => {
    if (this.state.Status !== 'READY') return;
    const reqBody = new messages.CreateDocReq({
      UserID: this.props.UserID,
      Content: serializeDoc(this.state.NewDoc),
    });

    try {
      const resp = await window.fetch(
        `${this.props.DocsvcHostExternal}/rpc/messages.CreateDocReq`,
        {
          method: 'post',
          mode: 'cors',
          cache: 'no-cache',
          headers: {
            Authorization: this.props.Tk,
          },
          body: JSON.stringify(messages.CreateDocReq.toObject(reqBody)),
        },
      );
      if (resp.ok) {
        const body = await resp.json();
        this.setState({
          Status: 'COMPLETE',
          // TODO: any
          NewDocTk: await body.Tk,
          NewDocSHA: await body.SHA,
        });
      } else {
        this.setState({
          Status: 'FAILED',
          message: 'failed to create initiative',
        });
      }
    } catch (err) {
      this.setState({
        Status: 'FAILED',
        message:
          'Sorry, something went wrong with your network request. Please check your connection and try again',
      });
    }
  };

  componentDidMount() {
    this.setState({Status: 'LOADING'});
    this.loadDoc();
  }

  componentDidUpdate(prevProps: Props, prevState: State) {
    if (this.state.Status === 'READY' && prevState.Status !== 'READY') {
      //this.editorView = initProsemirror(
      //  this.props.ProseMirrorEl,
      //  JSON.parse(this.state.OldDoc),
      //);
    }

    if (this.state.Status === 'COMPLETE') {
      const issueIDParam = this.props.IssueID
        ? `issue_id=${this.props.IssueID}&`
        : '';
      window.location.href = `/initiative/new.html?area_id=${this.props.AreaID}&${issueIDParam}tk=${this.state.NewDocTk}`;
    }
  }

  render() {
    if (this.state.Status === 'READY') {
      return (
        <div>
          <AreaEditor
            OldDoc={this.state.OldDoc}
            NewDoc={this.state.NewDoc}
            onUpdateDoc={(newDoc: Doc) => {
              this.setState({NewDoc: newDoc});
            }}
          />
          <button onClick={() => this.createDoc()}>Next...</button>
        </div>
      );
    }
    return <div>Loading: {this.state.Status}</div>;
  }
}

export type InitNewInitiativePageArgs = {
  RootEl: HTMLElement;
  DocsvcHostExternal: string;
  AreaID: string;
  UserID: string;
  IssueID?: string;
  DocSHA: string;
  Tk: string;
};

export const initNewInitiativePage = (args: InitNewInitiativePageArgs) => {
  render(
    <NewInitiativePage
      DocsvcHostExternal={args.DocsvcHostExternal}
      AreaID={args.AreaID}
      IssueID={args.IssueID}
      UserID={args.UserID}
      DocSHA={args.DocSHA}
      Tk={args.Tk}
    />,
    args.RootEl,
  );
};
