import { useState, useEffect } from "preact/hooks";
import { CreateRevReqBody } from "messages";
import { callCreateRev, callGet } from "./rpc";

type ID = string;

export type Section = {
  id: ID;
  content: string;
};

export type Doc = {
  sections: { [key: string]: Section };
  order: Array<ID>;
};

export const parseDoc = (docStr: string): Doc => {
  const parser = new window.DOMParser();
  const doc = parser.parseFromString(docStr, "application/xml");
  const article = doc.firstElementChild;
  if (article === null) {
    // TODO: invariant
    throw new Error("article missing from document");
  }
  const out: Doc = {
    sections: {},
    order: [],
  };

  // and article is always a flat collection of sections
  let section = article.firstElementChild;
  while (section) {
    const id = section.attributes.getNamedItem("id");
    if (id === null) {
      // TODO: invariant
      throw new Error("section does not have a id");
    }
    const content = section.innerHTML;
    out.sections[id.value] = {
      id: id.value,
      content: content,
    };
    out.order.push(id.value);
    section = section.nextElementSibling;
  }
  return out;
};

export const serializeDoc = (doc: Doc): string => {
  const articleEl = document.createElement("article");
  for (const sectionID of doc.order) {
    const section = doc.sections[sectionID];
    const sectionEl = document.createElement("section");
    sectionEl.setAttribute("id", section.id);
    sectionEl.innerHTML = section.content;
    articleEl.append(sectionEl);
  }
  const wrapper = document.createElement("div");
  wrapper.append(articleEl);
  return wrapper.innerHTML;
};

export const compareDocs = (docA: Doc, docB: Doc) => {
  if (docA.order.length !== docB.order.length) return false;

  for (let i = 0; i < docA.order.length; i++) {
    if (docA.order[i] !== docB.order[i]) return false;
  }

  for (const sectionID of Object.keys(docA.sections)) {
    if (
      docA.sections[sectionID].content !== docB.sections[sectionID]?.content
    ) {
      return false;
    }
  }

  return true;
};

abstract class AbstractDocState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly data: TData;

  public constructor(setState: (newState: TStateUnion) => void, Data: TData) {
    this.setState = setState;
    this.data = Data;
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
  newDocTk: string;
  newDocSHA: string;
} & DocReadyData;
export class DocStateComplete extends AbstractDocState<
  DocState,
  DocCompleteData
> {}

type DocFailedData = { readonly message: string };
export class DocStateFailed extends AbstractDocState<DocState, DocFailedData> {}

type DocReadyData = {
  readonly sha: string;
  readonly oldDoc: Doc;
  readonly newDoc: Doc;
};

export class DocStateReady extends AbstractDocState<DocState, DocReadyData> {
  public updateDoc = (newDoc: Doc) => {
    this.setState(
      new DocStateReady(this.setState, {
        sha: this.data.sha,
        oldDoc: this.data.oldDoc,
        newDoc: newDoc,
      })
    );
  };

  public updateSection = (sectionID: string, content: string) => {
    this.updateDoc({
      ...this.data.newDoc,
      sections: {
        ...this.data.newDoc.sections,
        [sectionID]: {
          id: sectionID,
          content: content,
        },
      },
    });
  };

  public submitDocRev = async (docsvcHostExternal: string, tk: string) => {
    this.setState(new DocStateSubmitting(this.setState, this.data));
    const reqBody: CreateRevReqBody = {
      content: serializeDoc(this.data.newDoc),
    };

    const result = await callCreateRev(docsvcHostExternal, reqBody, tk);
    if (result.ok) {
      this.setState(
        new DocStateComplete(this.setState, {
          ...this.data,
          newDocTk: result.resp.tk,
          newDocSHA: result.resp.sha,
        })
      );
    } else {
      this.setState(
        new DocStateFailed(this.setState, {
          message: result.msg,
        })
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
  baseDocSHA: string
): DocState => {
  const [state, setState] = useState(
    // TODO: you can't pass setState yet cos it doesn't exist.
    new DocStateIdle((newState: DocState) => undefined, {})
  );
  useEffect(() => {
    setState(new DocStateLoading(setState, {}));

    const loadDoc = async () => {
      const result = await callGet(docsvcHostExternal, { sha: baseDocSHA }, tk);
      if (result.ok) {
        const doc = parseDoc(result.resp.content);
        setState(
          new DocStateReady(setState, {
            oldDoc: doc,
            newDoc: doc,
            sha: baseDocSHA,
          })
        );
        return;
      } else {
        setState(
          new DocStateFailed(setState, {
            message: result.msg,
          })
        );
      }
    };
    loadDoc();
  }, [baseDocSHA]);

  return state;
};
