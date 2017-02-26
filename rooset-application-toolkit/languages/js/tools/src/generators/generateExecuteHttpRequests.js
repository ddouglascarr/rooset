const { getClassNameFromType } = require("../utils");

module.exports = (queryDecls, config) => {
  let factoryImports = [];
  let typeImports = [];
  const caseStatements = queryDecls.map(
    d => `
    case "${d.type}_REQUEST":
      return execute${getClassNameFromType(d.type)}HttpRequest(action)`);

  const factoryFunctions = queryDecls.map(d => {
    const className = getClassNameFromType(d.type + "_REQUEST");
    const errorClassName = getClassNameFromType(d.type + "_ERROR");
    const responseClassName = getClassNameFromType(d.type + "_RESPONSE");
    factoryImports = factoryImports.concat([
      `build${className}`,
      `build${errorClassName}`,
      `build${responseClassName}`
    ]);
    typeImports = typeImports.concat([className]);

    return `
async function execute${getClassNameFromType(d.type)}HttpRequest(
    action: ${className}) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("${d.httpUri}", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return build${responseClassName}(json);
  return build${errorClassName}(json.payload);
}`;
  });

  return `
// @flow
/* eslint-disable no-unused-vars */
import type { Action, HttpAction } from "../Action";
import { template } from "lodash";
import {
  ${factoryImports.join(",\n  ")}
} from "./QueryActions";
import type {
  QueryRequest,
  ${typeImports.join(",\n  ")}
} from "./QueryActions";

export default async function(action: Action) :Promise<HttpAction> {
  switch (action.type) {
${caseStatements.join("")}
    default:
      throw new Error("Not an HttpAction: " + action.type);
  }
}

${factoryFunctions.join("")}

function applyUriTemplate(uriTmpl, vars) {
  const opts = { interpolate:  /\{(.+?)\}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
`;
};
