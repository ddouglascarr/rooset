const generateQueryRequestAction = require("./generateQueryRequestAction");
const generateQueryResponseAction = require("./generateQueryResponseAction");
const generateQueryErrorAction = require("./generateQueryErrorAction");

const { getClassNameFromType } = require("../utils");

module.exports = (queryDecls, config) => {
  return `
// @flow
/* eslint-disable no-unused-vars */
import type {
  Uuid,
  Date,
  Area,
  ExceptionPayload,
  Initiative,
  IssueState,
  KeyNamePair,
  Member,
  Ullong,
  Policy,
} from '../../types';
import { template } from "lodash";
import HttpRequest from "../../sys/HttpRequest";


${queryDecls.map(d => generateQueryResponseAction(d, config)).join("\n")}
${queryDecls.map(d => generateQueryErrorAction(d, config)).join("\n")}
${queryDecls.map(d => generateQueryRequestAction(d, config)).join("\n")}

export type QueryRequest =
  | ${queryDecls.map(d => getClassNameFromType(d.type + "_REQUEST")).join("\n  | ")}

export type QueryResponse =
  | ${queryDecls.map(d => getClassNameFromType(d.type + "_RESPONSE")).join("\n  | ")}

export type QueryError =
  | ${queryDecls.map(d => getClassNameFromType(d.type + "_ERROR")).join("\n  | ")}

export type QueryAction = QueryRequest | QueryResponse | QueryError;
`;
};
