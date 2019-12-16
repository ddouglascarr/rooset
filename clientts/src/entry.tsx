import { initProsemirror, initInitiativeForm, logProsemirrorState } from './editor';

console.log('bundle loaded');

declare module global {
  function logProsemirrorState(): void;
}
global.logProsemirrorState = logProsemirrorState;

initProsemirror(document.getElementById('initiative-editor')!);

const initiativeForm = document.getElementById('initiative-form');
if (initiativeForm && initiativeForm instanceof HTMLFormElement) {
  initInitiativeForm(initiativeForm);
}
