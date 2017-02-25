// @flow
import { pick, extend } from "lodash";

export type HttpMethod = "GET" | "PUT" | "POST" | "DELETE";

export default class HttpRequest {
  method: HttpMethod;
  path: string;
  body: ?Object;
  constructor(method: HttpMethod, path: string, body: ?Object) {
    this.method = method;
    this.path = path;
    this.body = body;
  }
  execute(): Promise<Response> {
    const { method, path, body } = this;
    const req: Request = new Request(this.path, {
      method,
      body,
      headers: new Headers({ "Content-Type": "application/json" }),
      credetials: "same-origin",
    });
    return fetch(req);
  }
}
