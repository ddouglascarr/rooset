// tools for parsing rooset docs

type ID = string;
type SHA = string;

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
