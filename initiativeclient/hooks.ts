import { useState, useEffect } from "preact/hooks";
import {
  useDocState,
  DocStateFailed,
  DocStateReady,
  DocStateComplete,
  DocStateSubmitting,
} from "./doc";
import {
  useDescState,
  DescStateReady,
  DescStateSubmitting,
  DescStateFailed,
  DescStateComplete,
} from "./desc";

abstract class AbstractPageState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly data: TData;

  public constructor(setState: (newState: TStateUnion) => void, data: TData) {
    this.setState = setState;
    this.data = data;
  }
}

export class Idle extends AbstractPageState<PageState, {}> {}
export class Failed extends AbstractPageState<
  PageState,
  {
    readonly message: string;
  }
> {}
export class Loading extends AbstractPageState<PageState, {}> {}

export class RevView extends AbstractPageState<
  PageState,
  {
    // The document is viewable, and must be manually put into edit mode by user
    readonly rev: DocStateReady;
  }
> {
  public startEdit = () => {
    this.setState(new RevEdit(this.setState, this.data));
  };
}

export class RevEdit extends AbstractPageState<
  PageState,
  {
    readonly rev: DocStateReady;
  }
> {
  public cancel = () => {
    this.setState(new RevView(this.setState, { rev: this.data.rev }));
  };
}

export class RevReady extends AbstractPageState<
  PageState,
  {
    // Revision has been edited and is ready for submission.
    readonly rev: DocStateReady;
  }
> {
  public editMore = () => {
    this.setState(new RevView(this.setState, { rev: this.data.rev }));
  };
}

export class RevSubmitting extends AbstractPageState<
  PageState,
  {
    readonly rev: DocStateSubmitting;
  }
> {}

export class DescReady extends AbstractPageState<
  PageState,
  {
    readonly rev: DocStateComplete;
    readonly desc: DescStateReady;
    readonly title: string;
  }
> {
  public setTitle = (title: string) => {
    this.setState(
      new DescReady(this.setState, {
        rev: this.data.rev,
        desc: this.data.desc,
        title: title,
      })
    );
  };
}

export class DescSubmitting extends AbstractPageState<
  PageState,
  {
    readonly rev: DocStateComplete;
    readonly desc: DescStateSubmitting;
    readonly title: string;
  }
> {}

export class Review extends AbstractPageState<
  PageState,
  {
    readonly rev: DocStateComplete;
    readonly desc: DescStateComplete;
    readonly title: string;
  }
> {}

type PageState =
  | Idle
  | Failed
  | Loading
  | RevView
  | RevEdit
  | RevReady
  | RevSubmitting
  | DescReady
  | DescSubmitting
  | Review;

export const useInitiativePageState = (
  docsvcHostExternal: string,
  revTk: string,
  descTk: string,
  baseRevDocSHA: string,
  baseDescDocSHA: string
): PageState => {
  const [state, setState] = useState(
    // can't pass setState cost it don't exist yet.
    new Idle((newState: PageState) => undefined, {})
  );
  const revState = useDocState(docsvcHostExternal, revTk, baseRevDocSHA);
  const descState = useDescState(docsvcHostExternal, descTk, baseDescDocSHA);

  useEffect(() => {
    setState(new Loading(setState, {}));
  }, [baseRevDocSHA, baseDescDocSHA]);

  useEffect(() => {
    // handle doc loading failure
    if (!(state instanceof Failed) && revState instanceof DocStateFailed) {
      setState(new Failed(setState, { message: revState.data.message }));
    }
    if (!(state instanceof Failed) && descState instanceof DescStateFailed) {
      setState(new Failed(setState, { message: descState.data.message }));
    }

    // progress to ready when base docs are loaded
    if (
      state instanceof Loading &&
      revState instanceof DocStateReady &&
      descState instanceof DescStateReady
    ) {
      setState(new RevView(setState, { rev: revState }));
      return;
    }

    // progression to edit is triggered by RevView.startEdit()

    if (
      state instanceof RevEdit &&
      revState instanceof DocStateReady &&
      revState.data.newDoc !== revState.data.oldDoc
    ) {
      setState(new RevReady(setState, { rev: revState }));
    }

    // detect rev submission
    if (state instanceof RevReady && revState instanceof DocStateSubmitting) {
      setState(new RevSubmitting(setState, { rev: revState }));
      return;
    }

    if (descState instanceof DescStateReady) {
      if (
        state instanceof RevSubmitting &&
        revState instanceof DocStateComplete
      ) {
        setState(
          new DescReady(setState, { rev: revState, desc: descState, title: "" })
        );
        return;
      }
    }

    if (
      state instanceof DescReady &&
      revState instanceof DocStateComplete &&
      descState instanceof DescStateSubmitting
    ) {
      setState(
        new DescSubmitting(setState, {
          rev: revState,
          desc: descState,
          title: state.data.title,
        })
      );
      return;
    }

    if (
      state instanceof DescSubmitting &&
      revState instanceof DocStateComplete &&
      descState instanceof DescStateComplete
    ) {
      setState(
        new Review(setState, {
          rev: revState,
          desc: descState,
          title: state.data.title,
        })
      );
    }
  }, [state, revState, descState]);

  return state;
};
