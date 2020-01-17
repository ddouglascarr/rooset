import {schema} from 'prosemirror-schema-basic';
import {Node} from 'prosemirror-model';

export type ValidateResp = {
  ok: boolean;
  errors: Array<string>;
};

export const validate = (doc: {}): ValidateResp => {
  try {
    const node = Node.fromJSON(schema, doc);
    node.check();
    return {ok: true, errors: []};
  } catch (err) {
    return {ok: false, errors: [`${err}`]};
  }
};
