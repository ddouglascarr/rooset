const generateQueryRequestAction = require("./generateQueryRequestAction");

module.exports = (queryDecls, config) => {
  return `
// @flow
import type {
  Uuid,
  Date,
  Area,
} from '../../types';

${queryDecls.map(d => generateQueryRequestAction(d, config)).join("\n")}

`;
};
