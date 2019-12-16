import { initProsemirror, initInitiativeForm, logProsemirrorState } from './editor';

initProsemirror();

declare module global {
  function logProsemirrorState(): void;
}

global.logProsemirrorState = logProsemirrorState;
console.log('bundle loaded');

const initiativeForm = document.getElementById('initiative-form');
if (initiativeForm && initiativeForm instanceof HTMLFormElement) {
  initInitiativeForm(initiativeForm);
}
