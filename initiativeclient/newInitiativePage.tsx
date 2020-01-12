import {render, h, Component} from 'preact';
import {messages} from 'messages';

type Props = {
  tk: string;
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
  | {status: 'FAILED'; message: string};

class NewInitiativePage extends Component<Props, State> {
  constructor() {
    super();
    this.state = {status: 'IDLE' as const};
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

  async componentDidMount() {
    this.setState({status: 'LOADING_LIST'});
    await this.loadDoc();
  }

  render() {
    if (this.state.status === 'READY') {
      return <div>{this.state.doc}</div>;
    }
    return <div>Hello World: {this.state.status}</div>;
  }
}

export const initNewInitiativePage = (args: {
  fileListEl: HTMLElement;
  proseMirrorEl: HTMLElement;
  tk: string;
}) => {
  render(
    <NewInitiativePage proseMirrorEl={args.proseMirrorEl} tk={args.tk} />,
    args.fileListEl,
  );
};
