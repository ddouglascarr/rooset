import {useState, useEffect} from 'preact/hooks';
import {callCreateDesc, callGet} from './rpc';

export type Desc = {
  Content: string;
};

abstract class AbstractDescState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly Data: TData;

  public constructor(setState: (newState: TStateUnion) => void, Data: TData) {
    this.setState = setState;
    this.Data = Data;
  }
}

export class DescStateIdle extends AbstractDescState<DescState, {}> {
  public async loadDescTemplate(
    setState: (newState: DescState) => void,
    docsvcHostExternal: string, 
    tk: string,
    sHA: string, 
  ) {
    setState(new DescStateLoading(setState, {}));
    const result = await callGet(
      docsvcHostExternal,
      { SHA: sHA },
      tk,
    );
    if (result.ok) {
      setState(
        new DescStateReady(setState, {
          Desc: result.resp,
          DescTemplate: result.resp,
        })
      );
    } else {
      setState(
        new DescStateFailed(setState, { Message: result.msg })
      );
    }
  }
}

export class DescStateLoading extends AbstractDescState<DescState, {}> {}

type DescStateSubmittingData = DescReadyData;
export class DescStateSubmitting extends AbstractDescState<
  DescState,
  DescStateSubmittingData
> {}

type DocCompleteData = {
  NewDescTk: string;
} & DescReadyData ;
export class DescStateComplete extends AbstractDescState<
  DescState,
  DocCompleteData
> {}

type DescFailedData = {readonly Message: string};
export class DescStateFailed extends AbstractDescState<DescState, DescFailedData> {}

type DescReadyData = {
  readonly DescTemplate: Desc;
  readonly Desc: Desc;
};

export class DescStateReady extends AbstractDescState<DescState, DescReadyData> {
  public updateDescContent = (content: string) => {
    this.setState(
      new DescStateReady(this.setState, {
        ...this.Data,
        Desc: { Content: content },
      }),
    );
  };

  public submitDesc = async (
    docsvcHostExternal: string,
    tk: string,
  ) => {
    this.setState(new DescStateSubmitting(this.setState, this.Data));

    const result = await callCreateDesc(
      docsvcHostExternal,
      { Content: this.Data.Desc.Content },
      tk,
    );
    if (result.ok) {
      this.setState(
        new DescStateComplete(this.setState, {
          ...this.Data,
          NewDescTk: result.resp.Tk,
        }),
      );
    } else {
      this.setState(
        new DescStateFailed(this.setState, {
          Message: result.msg,
        }),
      );
    }
  };
}

export type DescState =
  | DescStateIdle
  | DescStateLoading
  | DescStateReady
  | DescStateFailed
  | DescStateSubmitting
  | DescStateComplete;

export const useDescState = (
  docsvcHostExternal: string,
  tk: string,
  descTemplateSHA: string,
): DescState => {
  const [state, setState] = useState<DescState>(
    // TODO: you can't pass setState yet cos it doesn't exist.
    new DescStateIdle((newState: DescState) => undefined, {}),
  );

  useEffect(() => {
    if (state instanceof DescStateIdle) {
      state.loadDescTemplate(setState, docsvcHostExternal, tk, descTemplateSHA);
    }
  }, [descTemplateSHA]);

  return state;
};
