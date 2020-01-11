import {schema} from 'prosemirror-schema-basic';
import {EditorState} from 'prosemirror-state';
import {EditorView} from 'prosemirror-view';
import {undo, redo, history} from 'prosemirror-history';
import {keymap} from 'prosemirror-keymap';
import {baseKeymap} from 'prosemirror-commands';
import {messages} from 'messages';

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
export const initInitiativeForm = (el: HTMLFormElement, tk: string) => {
  el.onsubmit = async e => {
    e.preventDefault();
    const reqBody = new messages.NewInitiativeReq({
      FileActions: [
        {
          Action: 0,
          FileName: 'test_file',
          Content: JSON.stringify(view.state.doc),
        },
      ],
    });
    const resp = await window.fetch('http://localhost:8080/new-initiative', {
      method: 'post',
      mode: 'cors',
      cache: 'no-cache',
      headers: {
        Authorization: tk,
      },
      body: JSON.stringify(messages.NewInitiativeReq.toObject(reqBody)),
    });

    if (resp.ok) {
      console.log('yay', resp);
    } else {
      console.log('boo', resp);
    }
  };
};

export const logProsemirrorState = () => {
  console.log('doc', JSON.stringify(view.state.doc, null, 2));
};
