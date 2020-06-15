import {h, render} from 'preact';
import {
  NewInitiativePage,
  NewInitiativePageProps,
} from './components/NewInitiativePage';

declare module global {
  function initNewInitiativePage(args: InitNewInitiativePageArgs): void;
}

export type InitNewInitiativePageArgs = NewInitiativePageProps & {
  RootEl: HTMLElement;
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
      OpenAdmittedSections={args.OpenAdmittedSections}
    />,
    args.RootEl,
  );
};

global.initNewInitiativePage = initNewInitiativePage;
