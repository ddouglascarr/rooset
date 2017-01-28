const { last, upperFirst, camelCase, snakeCase, upperCase } = require('lodash');

module.exports = {
  getTypenameFromRef: (ref) => last(ref.$ref.split('/')),
  getMsgTypeFromSchema: (schema) => schema.properties.type.enum[0],
  generateClassnameFromMsgType: (type) => upperFirst(camelCase(type)),
  generateFilenameFromMsgType: (type) => `${upperFirst(camelCase(type))}`,
  generateTestLabel: (jsonLabel) => snakeCase(upperCase(jsonLabel)),
};
