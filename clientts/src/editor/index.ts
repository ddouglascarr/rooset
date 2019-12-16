import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';

export const initProsemirror = () => {
  const state = EditorState.create({schema});
  new EditorView(document.getElementById('initiative-editor')!, {
    state,
  });
};
