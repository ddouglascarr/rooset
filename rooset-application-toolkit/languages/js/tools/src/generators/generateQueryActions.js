const generateQueryRequestAction = require("./generateQueryRequestAction");
const generateQueryResponseAction = require("./generateQueryResponseAction");
const generateQueryErrorAction = require("./generateQueryErrorAction");

module.exports = (queryDecls, config) => {
  return `
// @flow
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


${queryDecls.map(d => generateQueryResponseAction(d, config)).join("\n")}
${queryDecls.map(d => generateQueryRequestAction(d, config)).join("\n")}
${queryDecls.map(d => generateQueryErrorAction(d, config)).join("\n")}

function applyUriTemplate(uriTmpl, vars) {
  const opts = { interpolate:  /\{(.+?)\}/g };
  const tmpl = template(uriTmpl, opts);
  return tmpl(vars);
}
`;
};
