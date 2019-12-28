import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';
import {undo, redo, history} from 'prosemirror-history';
import {keymap} from 'prosemirror-keymap';
import {baseKeymap} from 'prosemirror-commands';

let view: EditorView;

export const initProsemirror = (el: Element) => {
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

  view = new EditorView(el, {
    state,
  });
};

// TODO: post to the git service instead
export const initInitiativeForm = (el: HTMLFormElement) => {
  el.onsubmit = e => {
    e.preventDefault();
    const fields = el.elements;
    for (let i = 0; i < fields.length; i++) {
      const field = fields[i];
      if (field instanceof HTMLInputElement && field.name === 'doc') {
        console.log('docField', field);
        field.value = JSON.stringify(view.state.doc);
      }
    }
    el.submit();
  };
};

export const logProsemirrorState = () => {
  console.log('doc', JSON.stringify(view.state.doc, null, 2));
};
