import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';
import {undo, redo, history} from "prosemirror-history"
import {keymap} from "prosemirror-keymap"
import {baseKeymap} from "prosemirror-commands"

let view: EditorView;

export const initProsemirror = () => {
  const state = EditorState.create({
    schema,
    plugins: [
      history(),
      keymap({
        'Mod-z': undo,
        'Mod-y': redo,
      }),
      keymap(baseKeymap),
    ],
  });

  view = new EditorView(document.getElementById('initiative-editor')!, {
    state,
  });
};

export const logProsemirrorState = () => {
  console.log('doc', JSON.stringify(view.state.doc, null, 2));
};
