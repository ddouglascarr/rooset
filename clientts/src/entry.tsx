import { initProsemirror, logProsemirrorState } from './editor';

initProsemirror();

declare module global {
  function logProsemirrorState(): void;
}

global.logProsemirrorState = logProsemirrorState;
console.log('bundle loaded');

