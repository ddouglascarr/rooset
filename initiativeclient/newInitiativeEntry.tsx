import {logProsemirrorState} from 'editor';
import {initNewInitiativePage} from './newInitiativePage';

console.log('bundle loaded');

declare module global {
  function logProsemirrorState(): void;
  function initNewInitiativePage(args: {
    fileListEl: HTMLElement;
    proseMirrorEl: HTMLElement;
    tk: string;
  }): void;
}
global.logProsemirrorState = logProsemirrorState;
global.initNewInitiativePage = initNewInitiativePage;

// global.initProsemirror(document.getElementById('initiative-editor')!);

// const initiativeForm = document.getElementById('initiative-form');
// if (initiativeForm && initiativeForm instanceof HTMLFormElement) {
//   initInitiativeForm(initiativeForm, global.rooset_editor_jwt);
// }
