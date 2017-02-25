const typenames = require("../templates/typenameMap");
const { camelCase, upperFirst, map } = require("lodash");

module.exports = (query, config) => {
  const requestType = `${query.type}_REQUEST`;
  const responseType = `${query.type}_RESPONSE`;
  const errorType = `${query.type}_ERROR`;

  const className = upperFirst(camelCase(requestType));
  const responseClassName = upperFirst(camelCase(responseType));
  const errorClassName = upperFirst(camelCase(errorType));
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

  buildResponse(payload: ${responseClassName + "Payload"})
    : ${responseClassName}
  {
    return new ${responseClassName}(payload);
  }

  buildError(payload: ExceptionPayload) : ${errorClassName} {
    return new ${errorClassName}(payload);
  }
}
`;
};
