const typenames = require("../templates/typenameMap");
const { camelCase, map } = require("lodash");

module.exports = (query, config) => {
  const requestType = `${query.type}_REQUEST`;
  const className = camelCase(requestType);
  const payloadDecl = map(query.req, (v, k) => `  ${k}: ${typenames[v.type]}`);
  return `
export type ${requestType} = "${requestType}";

type ${className}Payload = {|
${payloadDecl.join(",\n")}
|};

export class ${className} {
  type: ${requestType};
  payload: ${className}Payload;
  isHttpQueryAction: true;
  constructor(payload: ${className}Payload) {
    this.payload = payload;
    this.type = "${requestType}";
    this.isHttpQueryAction = true;
  }
  getHttpUri(): string {
    return applyUriTemplate("${query.httpUri}", this.payload);
  }
}
`;
};
