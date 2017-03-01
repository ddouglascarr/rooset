// @flow
import type { Uuid } from "../types";

export type PublicUserData = {|
  id: Uuid,
  username: String,
  displayName: String,
|};

export async function fetchPublicUserData(id: Uuid): Promise<?PublicUserData> {
  const req = new Request(`/users/${id.toString()}`, {
    method: "GET",
    headers: new Headers({ Accept: "application/json" }),
    include: "same-origin",
  });
  const resp = await fetch(req);
  if (resp.ok) return await resp.json();
  return null;
}
