const { camelCase, upperFirst } = require("lodash");

module.exports = (query, config) => {
  const errorType = `${query.type}_ERROR`;
  const className = upperFirst(camelCase(errorType));

  return `
export type ${errorType} = "${errorType}";

export class ${className} {
  type: ${errorType};
  payload: ExceptionPayload;
  error: true;

  constructor(payload: ExceptionPayload) {
    this.payload = payload;
    this.error = true;
  }
}
`;
};
