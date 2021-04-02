import {useState, useEffect} from 'preact/hooks';
import {CreateRevReqBody} from 'messages';
import {callCreateRev, callGet} from './rpc';

type ID = string;

export type Section = {
  ID: ID;
  Content: string;
};

export type Doc = {
  Sections: {[key: string]: Section};
  Order: Array<ID>;
};

export const parseDoc = (docStr: string): Doc => {
  const parser = new window.DOMParser();
  const doc = parser.parseFromString(docStr, 'application/xml');
  const article = doc.firstElementChild;
  if (article === null) {
    // TODO: invariant
    throw new Error('article missing from document');
  }
  const out: Doc = {
    Sections: {},
    Order: [],
  };

  // and article is always a flat collection of sections
  let section = article.firstElementChild;
  while (section) {
    const id = section.attributes.getNamedItem('id');
    if (id === null) {
      // TODO: invariant
      throw new Error('section does not have a id');
    }
    const content = section.innerHTML;
    out.Sections[id.value] = {
      ID: id.value,
      Content: content,
    };
    out.Order.push(id.value);
    section = section.nextElementSibling;
  }
  return out;
};

export const serializeDoc = (doc: Doc): string => {
  const articleEl = document.createElement('article');
  for (const sectionID of doc.Order) {
    const section = doc.Sections[sectionID];
    const sectionEl = document.createElement('section');
    sectionEl.setAttribute('id', section.ID);
    sectionEl.innerHTML = section.Content;
    articleEl.append(sectionEl);
  }
  const wrapper = document.createElement('div');
  wrapper.append(articleEl);
  return wrapper.innerHTML;
};

export const compareDocs = (docA: Doc, docB: Doc) => {
  if (docA.Order.length !== docB.Order.length) return false;

  for (let i = 0; i < docA.Order.length; i++) {
    if (docA.Order[i] !== docB.Order[i]) return false;
  }

  for (const sectionID of Object.keys(docA.Sections)) {
    if (
      docA.Sections[sectionID].Content !== docB.Sections[sectionID]?.Content
    ) {
      return false;
    }
  }

  return true;
};

abstract class AbstractDocState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly Data: TData;

  public constructor(setState: (newState: TStateUnion) => void, Data: TData) {
    this.setState = setState;
    this.Data = Data;
  }
}

export class DocStateIdle extends AbstractDocState<DocState, {}> {}
export class DocStateLoading extends AbstractDocState<DocState, {}> {}

type DocStateSubmittingData = DocReadyData;
export class DocStateSubmitting extends AbstractDocState<
  DocState,
  DocStateSubmittingData
> {}

type DocCompleteData = {
  NewDocTk: string;
  NewDocSHA: string;
} & DocReadyData;
export class DocStateComplete extends AbstractDocState<
  DocState,
  DocCompleteData
> {}

type DocFailedData = {readonly Message: string};
export class DocStateFailed extends AbstractDocState<DocState, DocFailedData> {}

type DocReadyData = {
  readonly SHA: string;
  readonly OldDoc: Doc;
  readonly NewDoc: Doc;
};

export class DocStateReady extends AbstractDocState<DocState, DocReadyData> {
  public updateDoc = (newDoc: Doc) => {
    this.setState(
      new DocStateReady(this.setState, {
        SHA: this.Data.SHA,
        OldDoc: this.Data.OldDoc,
        NewDoc: newDoc,
      }),
    );
  };

  public updateSection = (sectionID: string, content: string) => {
    this.updateDoc({
      ...this.Data.NewDoc,
      Sections: {
        ...this.Data.NewDoc.Sections,
        [sectionID]: {
          ID: sectionID,
          Content: content,
        },
      },
    });
  };

  public submitDocRev = async (
    docsvcHostExternal: string,
    tk: string,
  ) => {
    this.setState(new DocStateSubmitting(this.setState, this.Data));
    const reqBody: CreateRevReqBody = {
      Content: serializeDoc(this.Data.NewDoc),
    };

    const result = await callCreateRev(
      docsvcHostExternal,
      reqBody,
      tk,
    );
    if (result.ok) {
      this.setState(
        new DocStateComplete(this.setState, {
          ...this.Data,
          NewDocTk: result.resp.Tk,
          NewDocSHA: result.resp.SHA,
        }),
      );
    } else {
      this.setState(
        new DocStateFailed(this.setState, {
          Message: result.msg,
        }),
      );
    }
  };
}

export type DocState =
  | DocStateIdle
  | DocStateLoading
  | DocStateReady
  | DocStateFailed
  | DocStateSubmitting
  | DocStateComplete;

export const useDocState = (
  docsvcHostExternal: string,
  tk: string,
  baseDocSHA: string,
): DocState => {
  const [state, setState] = useState(
    // TODO: you can't pass setState yet cos it doesn't exist.
    new DocStateIdle((newState: DocState) => undefined, {}),
  );
  useEffect(() => {
    setState(new DocStateLoading(setState, {}));

    const loadDoc = async () => {
      const result = await callGet(
        docsvcHostExternal,
        { SHA: baseDocSHA},
        tk,
      );
      if (result.ok) {
        const doc = parseDoc(result.resp.Content);
        setState(
          new DocStateReady(setState, {
            OldDoc: doc,
            NewDoc: doc,
            SHA: baseDocSHA,
          }),
        );
        return;
      } else {
        setState(
          new DocStateFailed(setState, {
            Message: result.msg,
          }),
        );
      }
    };
    loadDoc();
  }, [baseDocSHA]);

  return state;
};
