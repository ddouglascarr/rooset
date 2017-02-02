const { map } = require('lodash');
const typenameMap = require(
    '../templates/typenameMap');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');

module.exports = function(schema) {
  const messageType = getMsgTypeFromSchema(schema);
  const className = generateClassnameFromMsgType(messageType);
  const payloadProps = schema.properties.payload.properties;

  const declarations = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `public final ${typenameMap[typename]} ${v};`;
  });

  const stdConstructorArgs = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `@JsonProperty("${v}") ${typenameMap[typename]} ${v}`;
  });

  const stdConstructorAssignments = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `this.${v} = ${v};`;
  });

  const stdConstructor = `
    public ${className}(
        ${stdConstructorArgs.join(',\n          ')})
    {
      ${stdConstructorAssignments.join('\n      ')}
    }
  `;

  return { declarations, stdConstructor };
}
