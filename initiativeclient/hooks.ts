import {useState, useEffect} from 'preact/hooks';
import {useDocState, DocStateFailed, DocStateReady, DocStateComplete, DocStateSubmitting} from './doc';
import {useDescState, DescStateReady, DescStateSubmitting, DescStateFailed, DescStateComplete } from './desc';

abstract class AbstractPageState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly Data: TData;

  public constructor(setState: (newState: TStateUnion) => void, Data: TData) {
    this.setState = setState;
    this.Data = Data;
  }
}

export class Idle extends AbstractPageState<PageState, {}> {}
export class Failed extends AbstractPageState<PageState, {
  readonly Message: string;
}> {}
export class Loading extends AbstractPageState<PageState, {}> {}

export class RevView extends AbstractPageState<PageState, {
    // The document is viewable, and must be manually put into edit mode by user
    readonly Rev: DocStateReady;

}> {
    public startEdit = () => {
      this.setState(new RevEdit(this.setState, this.Data));
    };
}

export class RevEdit extends AbstractPageState<PageState, {
    readonly Rev: DocStateReady;
}> {
    public cancel = () => {
      this.setState(new RevView(this.setState, { Rev: this.Data.Rev }));
    }
}

export class RevReady extends AbstractPageState<PageState, {
    // Revision has been edited and is ready for submission.
    readonly Rev: DocStateReady;
}> {
    public editMore = () => {
      this.setState(new RevView(this.setState, { Rev: this.Data.Rev }));
    }
}

export class RevSubmitting extends AbstractPageState<PageState, {
    readonly Rev: DocStateSubmitting;
}> {}

export class DescReady extends AbstractPageState<PageState, {
  readonly Rev: DocStateComplete;
  readonly Desc: DescStateReady;
  readonly Title: string;
}> {
  public setTitle = (title: string) => {
    this.setState(new DescReady(this.setState, {
      Rev: this.Data.Rev,
      Desc: this.Data.Desc, 
      Title: title,
    }));
  }
}

export class DescSubmitting extends AbstractPageState<PageState, {
    readonly Rev: DocStateComplete;
    readonly Desc: DescStateSubmitting;
    readonly Title: string;
}> {}

export class Review extends AbstractPageState<PageState, {
    readonly Rev: DocStateComplete;
    readonly Desc: DescStateComplete;
    readonly Title: string;
}> {}

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
    baseDescDocSHA: string,
): PageState => {
    const [state, setState] = useState(
        // can't pass setState cost it don't exist yet.
        new Idle((newState: PageState) => undefined, {}),
    );
    const revState = useDocState(docsvcHostExternal, revTk, baseRevDocSHA);
    const descState = useDescState(docsvcHostExternal, descTk, baseDescDocSHA);

    useEffect(() => {
        setState(new Loading(setState, {}));
    }, [baseRevDocSHA, baseDescDocSHA]);

    useEffect(() => {
        // handle doc loading failure
        if (!(state instanceof Failed) && (revState instanceof DocStateFailed)) {
          setState(new Failed(setState, { Message: revState.Data.Message }));
        }
        if (!(state instanceof Failed) && (descState instanceof DescStateFailed)) {
          setState(new Failed(setState, { Message: descState.Data.Message }));
        }

        // progress to ready when base docs are loaded
        if (
          state instanceof Loading && 
          revState instanceof DocStateReady && 
          descState instanceof DescStateReady
        ) {
            setState(new RevView(setState, { Rev: revState }));
            return;
        }

        // progression to edit is triggered by RevView.startEdit()
        
        if (
          state instanceof RevEdit && 
          revState instanceof DocStateReady && 
          revState.Data.NewDoc !== revState.Data.OldDoc
        ) {
          setState(new RevReady(setState, { Rev: revState }));
        }

        // detect rev submission
        if (state instanceof RevReady && revState instanceof DocStateSubmitting) {
            setState(new RevSubmitting(setState, { Rev: revState }));
            return;
        }

        if (descState instanceof DescStateReady) {
          if (state instanceof RevSubmitting && revState instanceof DocStateComplete) {
            setState(new DescReady(setState, { Rev: revState, Desc: descState, Title: "" }));
            return;
          }
        }

        if (
          state instanceof DescReady && 
          revState instanceof DocStateComplete && 
          descState instanceof DescStateSubmitting
        ) {
          setState(new DescSubmitting(setState, { Rev: revState, Desc: descState, Title: state.Data.Title}));
          return;
        }

        if (state instanceof DescSubmitting && revState instanceof DocStateComplete && descState instanceof DescStateComplete) {
          setState(new Review(setState, { Rev: revState, Desc: descState, Title: state.Data.Title }));
        }
    }, [state, revState, descState]);

    return state;
};
