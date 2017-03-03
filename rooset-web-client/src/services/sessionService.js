// @flow
import type { Uuid } from "../types";

export type PerformLoginResponse = {| username: string |};
export async function performLoginRequest(
  username: string,
  password: string,
): Promise<PerformLoginResponse> {
  const body = new FormData();
  body.set("username", username);
  body.set("password", password);
  await fetch("/login", {
    method: "POST",
    body,
    credentials: "same-origin"
  });
  return { username: "" };
}

export type GetSessionResponse = {|
  id: Uuid,
  username: string,
  displayName: string,
|};
export async function getSession(): Promise<?GetSessionResponse> {
  const httpResp = await fetch(new Request("/session", {
    headers: new Headers().set("Accept", "application/json"),
    method: "GET",
    credentials: "same-origin"
  }));
  if (httpResp.ok) {
    const body = await httpResp.json();
    return {
      username: body.username,
      id: body.id,
      displayName: body.displayName,
    };
  }
  return null;
}
