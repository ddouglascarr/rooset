import {render, h, Component} from 'preact';

type Props = {
  tk: string;
  proseMirrorEl: HTMLElement;
};

type State = {status: 'IDLE'} | {status: 'LOADING'};

class NewInitiativePage extends Component<Props, State> {
  constructor() {
    super();
    this.state = {status: 'IDLE' as const};
  }

  render() {
    return <div>Hello World</div>;
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
