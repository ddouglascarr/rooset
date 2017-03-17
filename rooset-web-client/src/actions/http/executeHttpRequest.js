
// @flow
/* eslint-disable no-unused-vars */
import type { Action, HttpAction } from "../Action";
import { template } from "lodash";
import {
  buildIssueQueryRequest,
  buildIssueQueryError,
  buildIssueQueryResponse,
  buildUnitDelegationsQueryRequest,
  buildUnitDelegationsQueryError,
  buildUnitDelegationsQueryResponse,
  buildUnitMemberQueryRequest,
  buildUnitMemberQueryError,
  buildUnitMemberQueryResponse,
  buildUnitQueryRequest,
  buildUnitQueryError,
  buildUnitQueryResponse,
  buildUnitUrlParameterNameQueryRequest,
  buildUnitUrlParameterNameQueryError,
  buildUnitUrlParameterNameQueryResponse
} from "./QueryActions";
import type {
  QueryRequest,
  IssueQueryRequest,
  UnitDelegationsQueryRequest,
  UnitMemberQueryRequest,
  UnitQueryRequest,
  UnitUrlParameterNameQueryRequest
} from "./QueryActions";

export default async function(action: Action) :Promise<HttpAction> {
  switch (action.type) {

    case "ISSUE_QUERY_REQUEST":
      return executeIssueQueryHttpRequest(action)
    case "UNIT_DELEGATIONS_QUERY_REQUEST":
      return executeUnitDelegationsQueryHttpRequest(action)
    case "UNIT_MEMBER_QUERY_REQUEST":
      return executeUnitMemberQueryHttpRequest(action)
    case "UNIT_QUERY_REQUEST":
      return executeUnitQueryHttpRequest(action)
    case "UNIT_URL_PARAMETER_NAME_QUERY_REQUEST":
      return executeUnitUrlParameterNameQueryHttpRequest(action)
    default:
      throw new Error("Not an HttpAction: " + action.type);
  }
}


async function executeIssueQueryHttpRequest(
    action: IssueQueryRequest) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("/issues/{id}", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return buildIssueQueryResponse(json);
  return buildIssueQueryError(json.payload);
}
async function executeUnitDelegationsQueryHttpRequest(
    action: UnitDelegationsQueryRequest) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("/units/{id}/members/{trusterId}/delegations", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return buildUnitDelegationsQueryResponse(json);
  return buildUnitDelegationsQueryError(json.payload);
}
async function executeUnitMemberQueryHttpRequest(
    action: UnitMemberQueryRequest) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("/units/{id}/members", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return buildUnitMemberQueryResponse(json);
  return buildUnitMemberQueryError(json.payload);
}
async function executeUnitQueryHttpRequest(
    action: UnitQueryRequest) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("/units/{id}", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return buildUnitQueryResponse(json);
  return buildUnitQueryError(json.payload);
}
async function executeUnitUrlParameterNameQueryHttpRequest(
    action: UnitUrlParameterNameQueryRequest) :Promise<HttpAction>
{
  const req = new Request(applyUriTemplate("/unit-url-parameter-names/{urlParameterName}", action.payload), {
    method: "GET",
    headers: new Headers({"Content-Type": "application/json"}),
    include: "same-origin",
  });
  const resp = await fetch(req);
  const json = await resp.json();
  if (resp.ok) return buildUnitUrlParameterNameQueryResponse(json);
  return buildUnitUrlParameterNameQueryError(json.payload);
}

function applyUriTemplate(uriTmpl, vars) {
  const opts = { interpolate:  /{(.+?)}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
