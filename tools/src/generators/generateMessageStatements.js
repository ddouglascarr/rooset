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


module.exports = function(schema, commandConstructorSchema) {
  const messageType = getMsgTypeFromSchema(schema);
  const classTypename = generateClassnameFromMsgType(messageType);
  const payloadProps = schema.properties.payload.properties;

  const declarations = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    return `const ${typenameMap[typename]} ${v};`;
  });

  const jsonConstructorAssignments = map(payloadProps, (ref, v) => {
    const typename = getTypenameFromRef(ref);
    if (!jsonConstructorAssignmentTemplates[typename]) {
      throw `jsonConstructorAssignmentTemplates.${typename} does not exist`;
    }
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

  const commandConstructor = commandConstructorSchema ?
      generateCommandConstructor(schema, commandConstructorSchema) : '';

  const commandConstructorImportStatement = commandConstructorSchema ?
      `#include "commands/${generateClassnameFromMsgType(getMsgTypeFromSchema(commandConstructorSchema))}.h"` : '';

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
    commandConstructor,
    commandConstructorImportStatement,
    serializeStatements,
  };
}

function generateCommandConstructor(schema, commandConstructorSchema) {
  const schemaProps = Object.keys(schema.properties.payload.properties);
  const cmdProps = Object.keys(commandConstructorSchema.properties.payload.properties);
  const params = schemaProps.map((prop) => {
    if (cmdProps.indexOf(prop) === -1) {
      throw `Event props must be a subset of command props. ${prop} is not in command`;
    }
    return `${prop}(c.${prop})`;
  });

  const messageType = getMsgTypeFromSchema(schema);
  const evtTypename = generateClassnameFromMsgType(getMsgTypeFromSchema(schema));
  const cmdTypename = generateClassnameFromMsgType(getMsgTypeFromSchema(commandConstructorSchema));
  return `
    ${evtTypename}(const ${cmdTypename}& c):
        ${params.join(',\n')}
    {}
  `;

}
