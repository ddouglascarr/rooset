import "preact/debug";

import {h, render} from 'preact';
import {
  NewInitiativePage,
  NewInitiativePageProps,
} from './components/NewInitiativePage';

declare module global {
  function initNewInitiativePage(rootEl: HTMLElement, args: NewInitiativePageProps): void;
}

export const initNewInitiativePage = (rootEl: HTMLElement, args: NewInitiativePageProps) => {
  render(
    <NewInitiativePage {...args} />,
    rootEl,
  );
};

global.initNewInitiativePage = initNewInitiativePage;
