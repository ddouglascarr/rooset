// tools for parsing rooset docs
import {useState, useEffect} from 'preact/hooks';
import {CreateDocReqBody, GetDocResp} from 'messages';

type ID = string;

export type Section = {
  ID: ID;
  Content: string;
};

export type Doc = {
  Sections: {[key: string]: Section};
  Order: Array<ID>;
};

export const parseDoc = (docStr: string) => {
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

export const serializeDoc = (doc: Doc) => {
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

  public submitDoc = async (
    docsvcHostExternal: string,
    tk: string,
  ) => {
    this.setState(new DocStateSubmitting(this.setState, this.Data));
    const reqBody: CreateDocReqBody = {
      Content: serializeDoc(this.Data.NewDoc),
    };

    try {
      const resp = await window.fetch(
        `${docsvcHostExternal}/rpc/docsvc.CreateDocReq`,
        {
          method: 'post',
          mode: 'cors',
          cache: 'no-cache',
          headers: {
            Authorization: tk,
          },
          body: JSON.stringify(reqBody),
        },
      );
      if (resp.ok) {
        const body = await resp.json();
        this.setState(
          new DocStateComplete(this.setState, {
            ...this.Data,
            NewDocTk: await body.Tk,
            NewDocSHA: await body.SHA,
          }),
        );
      } else {
        this.setState(
          new DocStateFailed(this.setState, {
            Message: 'failed to create initiative',
          }),
        );
      }
    } catch (err) {
      this.setState(
        new DocStateFailed(this.setState, {
          Message:
            'Sorry, something went wrong with your network request. Please check your connection and try again',
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
      const reqBody = {}; 
      try {
        const resp = await window.fetch(
          `${docsvcHostExternal}/rpc/docsvc.GetDocReq`,
          {
            method: 'post',
            mode: 'cors',
            cache: 'no-cache',
            headers: {
              Authorization: tk,
            },
            body: JSON.stringify(reqBody),
          },
        );
        if (resp.ok) {
          const body: GetDocResp = await resp.json();
          const docStr = body.Content;
          const doc = parseDoc(docStr);
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
              Message: 'failed',
            }),
          );
        }
      } catch (err) {
        setState(
          new DocStateFailed(setState, {
            Message: 'failed',
          }),
        );
      }
    };
    loadDoc();
  }, [baseDocSHA]);

  return state;
};
