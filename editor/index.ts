import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';
import {Node as ProseMirrorNode} from 'prosemirror-model';
import {undo, redo, history} from 'prosemirror-history';
import {keymap} from 'prosemirror-keymap';
import {baseKeymap} from 'prosemirror-commands';

export const initProsemirror = (el: Element, doc: {}): EditorView => {
  const state = EditorState.create({
    doc: ProseMirrorNode.fromJSON(schema, doc),
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

  const view = new EditorView(el, {
    state,
  });

  return view;
};
