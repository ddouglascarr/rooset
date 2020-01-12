import {initNewInitiativePage} from './newInitiativePage';

console.log('bundle loaded');

declare module global {
  function initNewInitiativePage(args: {
    fileListEl: HTMLElement;
    proseMirrorEl: HTMLElement;
    tk: string;
    areaID: number;
  }): void;
}

global.initNewInitiativePage = initNewInitiativePage;

// global.initProsemirror(document.getElementById('initiative-editor')!);

// const initiativeForm = document.getElementById('initiative-form');
// if (initiativeForm && initiativeForm instanceof HTMLFormElement) {
//   initInitiativeForm(initiativeForm, global.rooset_editor_jwt);
// }
