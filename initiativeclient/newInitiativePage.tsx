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
      blobList: Array<messages.IBlobRecord>;
      doc: messages.IBlob;
    }
  | {status: 'FAILED'; message: string};

class NewInitiativePage extends Component<Props, State> {
  constructor() {
    super();
    this.state = {status: 'IDLE' as const};
  }

  loadBlobList = async () => {
    const reqBody = messages.ListAreaBlobsReq.fromObject({
      SHA: 'master',
    });
    try {
      const resp = await window.fetch('http://localhost:8080/list-area-blobs', {
        method: 'post',
        mode: 'cors',
        cache: 'no-cache',
        headers: {
          Authorization: this.props.tk,
        },
        body: JSON.stringify(messages.ListAreaBlobsReq.toObject(reqBody)),
      });
      if (resp.ok) {
        const body = await resp.json();
        const reason = messages.ListAreaBlobsResp.verify(body);
        if (reason) {
          this.setState({
            status: 'FAILED',
            message: `invalid server response ${reason}`,
          });
          return;
        }
        this.setState({
          status: 'LOADING_DOC',
          blobList: messages.ListAreaBlobsResp.fromObject(body).BlobRecords,
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

  loadDocBlob = async (sha: string) => {
    if (this.state.status !== 'LOADING_DOC') return;
    const reqBody = messages.GetBlobReq.fromObject({
      SHA: sha,
    });
    try {
      const resp = await window.fetch('http://localhost:8080/get-blob', {
        method: 'post',
        mode: 'cors',
        cache: 'no-cache',
        headers: {
          Authorization: this.props.tk,
        },
        body: JSON.stringify(messages.ListAreaBlobsReq.toObject(reqBody)),
      });

      if (resp.ok) {
        const body = await resp.json();
        const reason = messages.GetBlobResp.verify(body);
        if (reason) {
          this.setState({
            status: 'FAILED',
            message: `invalid server response ${reason}`,
          });
          return;
        }
        this.setState({
          status: 'READY',
          blobList: this.state.blobList,
          doc: messages.GetBlobResp.fromObject(body).Blob!,
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
    await this.loadBlobList();
    if (this.state.status !== 'LOADING_DOC') return;
    // TODO: if not blobs in area, fail

    const sha = this.state.blobList[0] ? this.state.blobList[0].SHA : undefined;
    if (!sha) {
      this.setState({
        status: 'FAILED',
        message: 'Sorry, the area is not configured properly',
      });
      return;
    }

    await this.loadDocBlob(sha);
  }

  render() {
    if (this.state.status === 'READY') {
      return <div>{this.state.doc.Content}</div>;
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
