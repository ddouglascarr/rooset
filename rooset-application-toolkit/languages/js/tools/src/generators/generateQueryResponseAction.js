const typenames = require("../templates/typenameMap");
const { camelCase, map } = require("lodash");

module.exports = (query, config) => {
  const responseType = `${query.type}_RESPONSE`;
  const className = camelCase(responseType);
  const payloadDecl = map(query.resp, (v, k) => `  ${k}: ${typenames[v.type]}`);
  return `
export type ${responseType} = "${responseType}";

type ${className}Payload = {|
${payloadDecl.join(",\n")}
|};

export class ${className} {
  type: ${responseType};
  payload: ${className}Payload;

  constructor(payload: ${className}Payload) {
    this.payload = payload;
    this.type = "${responseType}";
  }
}

`;
};
