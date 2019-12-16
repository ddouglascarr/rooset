import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';
import {undo, redo, history} from "prosemirror-history"
import {keymap} from "prosemirror-keymap"

export const initProsemirror = () => {
  const state = EditorState.create({
    schema,
    plugins: [
      history(),
      keymap({
        'Mod-z': undo,
        'Mod-y': redo,
      }),
    ],
  });

  new EditorView(document.getElementById('initiative-editor')!, {
    state,
  });
};
