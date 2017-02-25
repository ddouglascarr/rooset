const { camelCase, upperFirst, map } = require("lodash");
const { getClassNameFromType, buildPayloadDeclaration } = require("../utils");

module.exports = (query, config) => {
  const requestType = `${query.type}_REQUEST`;

  const className = getClassNameFromType(requestType);
  const payloadDecl = buildPayloadDeclaration(query.req);
  return `
export type ${requestType} = "${requestType}";

type ${className}Payload = {|
${payloadDecl.join(",\n")}
|};

export type ${className} = {|
  type: ${requestType},
  payload: ${className}Payload,
|};

export function build${className}(payload: ${className}Payload) :${className} {
  return {
    type: "${requestType}",
    payload,
  };
}
`;
};
