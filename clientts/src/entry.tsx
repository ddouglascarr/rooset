import { initProsemirror, initInitiativeForm, logProsemirrorState } from './editor';

initProsemirror();

declare module global {
  function logProsemirrorState(): void;
}

global.logProsemirrorState = logProsemirrorState;
console.log('bundle loaded');

// TODO: don't use an assertion here
const initiativeForm = document.getElementById('initiative-form') as HTMLFormElement;
if (initiativeForm) {
  initInitiativeForm(initiativeForm);
}
