import { h, render } from "preact";

import {
  Props,
  CompetingInitiativePage,
} from "./components/CompetingInitiativePage";

declare module global {
  function initCompetingInitiativePage(rootEl: HTMLElement, props: Props): void;
}

const initCompetingInitiativePage = (rootEl: HTMLElement, props: Props) => {
  render(<CompetingInitiativePage {...props} />, rootEl);
};

global.initCompetingInitiativePage = initCompetingInitiativePage;
