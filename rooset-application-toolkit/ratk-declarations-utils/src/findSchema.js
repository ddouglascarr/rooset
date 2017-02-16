const { find } = require('lodash');
const generateSchemas = require('./generateSchemas');


module.exports = (baseSchema, messageCategory, srcPath, type) => {
  const schemas = generateSchemas(baseSchema, messageCategory, srcPath);
  const schema = find(schemas, (s) => s.type === type);
  if (!schema) throw new Error(
      `cannot find ${messageCategory} schema of type ${type}`);
  return schema;
};
