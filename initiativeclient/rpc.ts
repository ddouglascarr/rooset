import {
  CreateRevReqBody,
  CreateRevResp,
  GetResp,
  GetReqBody,
  CreateDescReqBody,
  CreateDescResp,
} from "messages";

type RPCResult<SuccessType> =
  | { ok: true; resp: SuccessType }
  | { ok: false; msg: string };

// TODO(ddc): these 3 methods could be done with a single generic.

export const callCreateRev = async (
  docsvcHostExternal: string,
  req: CreateRevReqBody,
  tk: string
): Promise<RPCResult<CreateRevResp>> => {
  try {
    const resp = await window.fetch(
      `${docsvcHostExternal}/rpc/docsvc.CreateRev`,
      {
        method: "post",
        mode: "cors",
        cache: "no-cache",
        headers: {
          Authorization: tk,
        },
        body: JSON.stringify(req),
      }
    );

    if (resp.ok) {
      const body: CreateRevResp = await resp.json();
      return { ok: true, resp: body };
    }
    return { ok: false, msg: "failed to create initiative" };
  } catch (err) {
    return {
      ok: false,
      msg:
        "something went wrong with your network request, check your connection and try again",
    };
  }
};

export const callGet = async (
  docsvcHostExternal: string,
  req: GetReqBody,
  tk: string
): Promise<RPCResult<GetResp>> => {
  try {
    const resp = await window.fetch(`${docsvcHostExternal}/rpc/docsvc.Get`, {
      method: "post",
      mode: "cors",
      cache: "no-cache",
      headers: {
        Authorization: tk,
      },
      body: JSON.stringify(req),
    });

    if (resp.ok) {
      const body: GetResp = await resp.json();
      return { ok: true, resp: body };
    }
    return { ok: false, msg: "failed to create initiative" };
  } catch (err) {
    return {
      ok: false,
      msg:
        "something went wrong with your network request, check your connection and try again",
    };
  }
};

export const callCreateDesc = async (
  docsvcHostExternal: string,
  req: CreateDescReqBody,
  tk: string
): Promise<RPCResult<CreateDescResp>> => {
  try {
    const resp = await window.fetch(
      `${docsvcHostExternal}/rpc/docsvc.CreateDesc`,
      {
        method: "post",
        mode: "cors",
        cache: "no-cache",
        headers: {
          Authorization: tk,
        },
        body: JSON.stringify(req),
      }
    );

    if (resp.ok) {
      const body: CreateDescResp = await resp.json();
      return { ok: true, resp: body };
    }
    return { ok: false, msg: "failed to create initiative" };
  } catch (err) {
    return {
      ok: false,
      msg:
        "something went wrong with your network request, check your connection and try again",
    };
  }
};
