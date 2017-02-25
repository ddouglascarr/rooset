const { map } = require("lodash");
const { getClassNameFromType, buildPayloadDeclaration } = require("../utils");

module.exports = (query, config) => {
  const responseType = `${query.type}_RESPONSE`;
  const className = getClassNameFromType(responseType);
  const payloadDecl = buildPayloadDeclaration(query.resp);
  return `
export type ${responseType} = "${responseType}";
type ${className}Payload = {|
${payloadDecl.join(",\n")}
|};

export type ${className} = {|
  type: ${responseType},
  payload: ${className}Payload,
|}

export function build${className}(payload: ${className}Payload) :${className} {
  return {
    type: "${responseType}",
    payload: payload,
  }
}

`;
};
