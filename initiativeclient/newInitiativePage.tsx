import {render, h} from 'preact';

const NewInitiativePage = ({}: {}) => <div>HelloWorld</div>;

export const initNewInitiativePage = (args: {
  fileListEl: HTMLElement;
  proseMirrorEl: HTMLElement;
  tk: string;
}) => {
  render(<NewInitiativePage />, args.fileListEl);
};
