import {initProsemirror, initInitiativeForm, logProsemirrorState} from 'editor';

console.log('bundle loaded');

declare module global {
  function logProsemirrorState(): void;
  const rooset_editor_jwt: string;
}
global.logProsemirrorState = logProsemirrorState;

initProsemirror(document.getElementById('initiative-editor')!);

const initiativeForm = document.getElementById('initiative-form');
if (initiativeForm && initiativeForm instanceof HTMLFormElement) {
  initInitiativeForm(initiativeForm, global.rooset_editor_jwt);
}
