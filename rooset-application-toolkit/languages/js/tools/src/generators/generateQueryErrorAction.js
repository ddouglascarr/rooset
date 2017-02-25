const { getClassNameFromType } = require("../utils");

module.exports = (query, config) => {
  const errorType = `${query.type}_ERROR`;
  const className = getClassNameFromType(errorType);

  return `
export type ${errorType} = "${errorType}";

export type ${className} = {
  type: ${errorType},
  error: true,
  payload: ExceptionPayload,
};

export function build${className}(payload: ExceptionPayload) {
  return {
    type: "${errorType}",
    error:true,
    payload,
  };
}
`;
};
