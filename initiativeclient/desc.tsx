import { useState, useEffect } from "preact/hooks";
import { callCreateDesc, callGet } from "./rpc";

export type Desc = {
  content: string;
};

abstract class AbstractDescState<TStateUnion, TData> {
  protected readonly setState: (newState: TStateUnion) => void;
  public readonly data: TData;

  public constructor(setState: (newState: TStateUnion) => void, data: TData) {
    this.setState = setState;
    this.data = data;
  }
}

export class DescStateIdle extends AbstractDescState<DescState, {}> {
  public async loadDescTemplate(
    setState: (newState: DescState) => void,
    docsvcHostExternal: string,
    tk: string,
    sHA: string
  ) {
    setState(new DescStateLoading(setState, {}));
    const result = await callGet(docsvcHostExternal, { sha: sHA }, tk);
    if (result.ok) {
      setState(
        new DescStateReady(setState, {
          desc: result.resp,
          descTemplate: result.resp,
        })
      );
    } else {
      setState(new DescStateFailed(setState, { message: result.msg }));
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
  newDescTk: string;
} & DescReadyData;
export class DescStateComplete extends AbstractDescState<
  DescState,
  DocCompleteData
> {}

type DescFailedData = { readonly message: string };
export class DescStateFailed extends AbstractDescState<
  DescState,
  DescFailedData
> {}

type DescReadyData = {
  readonly descTemplate: Desc;
  readonly desc: Desc;
};

export class DescStateReady extends AbstractDescState<
  DescState,
  DescReadyData
> {
  public updateDescContent = (content: string) => {
    this.setState(
      new DescStateReady(this.setState, {
        ...this.data,
        desc: { content: content },
      })
    );
  };

  public submitDesc = async (docsvcHostExternal: string, tk: string) => {
    this.setState(new DescStateSubmitting(this.setState, this.data));

    const result = await callCreateDesc(
      docsvcHostExternal,
      { content: this.data.desc.content },
      tk
    );
    if (result.ok) {
      this.setState(
        new DescStateComplete(this.setState, {
          ...this.data,
          newDescTk: result.resp.tk,
        })
      );
    } else {
      this.setState(
        new DescStateFailed(this.setState, {
          message: result.msg,
        })
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
  descTemplateSHA: string
): DescState => {
  const [state, setState] = useState<DescState>(
    // TODO: you can't pass setState yet cos it doesn't exist.
    new DescStateIdle((newState: DescState) => undefined, {})
  );

  useEffect(() => {
    if (state instanceof DescStateIdle) {
      state.loadDescTemplate(setState, docsvcHostExternal, tk, descTemplateSHA);
    }
  }, [descTemplateSHA]);

  return state;
};
