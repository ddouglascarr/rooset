const { map } = require('lodash');
const typenameMap = require(
    '../templates/typenameMap');
const jsonConstructorAssignmentTemplates = require(
    '../templates/jsonConstructorAssignmentTemplates');
const stdConstructorAssignmentTemplates = require(
    '../templates/stdConstructorAssignmentTemplates');
const serializeTemplates = require(
    '../templates/serializeTemplates');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');


module.exports = function(schema) {
  const messageType = getMsgTypeFromSchema(schema);
  const classTypename = generateClassnameFromMsgType(messageType);
  const payloadProps = schema.properties.payload.properties;

  const declarations = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `const ${typenameMap[typename]} ${v};`;
  });

  const jsonConstructorAssignments = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return jsonConstructorAssignmentTemplates[typename](v);
  });

  const jsonConstructor = `
        ${classTypename}(const Document& d) :
        ${jsonConstructorAssignments.join(',')}
        {}
  `;

  const stdConstructorArgs = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `${typenameMap[typename]} ${v}`;
  });

  const stdConstructorAssignments = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return stdConstructorAssignmentTemplates[typename](v, '');
  });

  const stdConstructor = `
        ${classTypename}(
            ${stdConstructorArgs.join(',\n            ')}) :
            ${stdConstructorAssignments.join(',\n            ')}
        {}
  `;

  const serializeStatements = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return serializeTemplates[typename](v);
  });

  return {
    declarations,
    jsonConstructorAssignments,
    jsonConstructor,
    stdConstructorArgs,
    stdConstructorAssignments,
    stdConstructor,
    serializeStatements,
  };
}
