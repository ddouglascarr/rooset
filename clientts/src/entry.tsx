import { initProsemirror, logProsemirrorState } from './editor';

initProsemirror();

setInterval(() => logProsemirrorState(), 10000);
console.log('bundle loaded');

