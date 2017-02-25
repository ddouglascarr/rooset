const generateQueryRequestAction = require("./generateQueryRequestAction");
const generateQueryResponseAction = require("./generateQueryResponseAction");

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

${queryDecls.map(d => generateQueryRequestAction(d, config)).join("\n")}

${queryDecls.map(d => generateQueryResponseAction(d, config)).join("\n")}
`;
};
