// @flow

export type PerformLoginResponse = {| username: string |}
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
  })
  return { username: "" };
}
