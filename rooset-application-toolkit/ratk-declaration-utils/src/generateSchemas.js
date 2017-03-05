const { merge, pick, includes } = require('lodash');
const fs = require('fs');
const glob = require('glob');
const ajv = require('ajv');
const YAML = require('yamljs');
const path = require('path');


const BASE_URL = 'https://rooset.org/schemas';
const typeRe = /([A-Z_]+)/;
const isYamlFilename = (filename) => filename.slice(filename.length - 4) === 'yaml';
const cache = {};


module.exports = (baseSchema, messageCategory, _srcPath) => {
  if (cache[messageCategory]) return cache[messageCategory];

  const srcPath = `${_srcPath}`;
  if (!includes(['commands', 'events', 'exceptions'], messageCategory)) {
    throw `${messageCategory} message category invalid`;
  }
  const jsonSrcFiles = glob.sync(`${srcPath}/*.json`, /**/
      { cwd: process.cwd() });
  const yamlSrcFiles = glob.sync(`${srcPath}/*.yaml`, /**/
      { cwd: process.cwd() });

  cache[messageCategory] = jsonSrcFiles.concat(yamlSrcFiles).map(mapSchemaFiles(baseSchema, messageCategory));
  return cache[messageCategory];
};


function mapSchemaFiles(baseSchema, messageCategory) {
  const isException = messageCategory === 'exceptions';
  return (filename) => {
    try {
      const messageSchema = isYamlFilename(filename) ?
          YAML.load(filename) : require(filename);
      const type = typeRe.exec(filename)[0];
      const required = [];
      const payload = Object.keys(messageSchema.properties)
          .reduce((memo, v) => {
            const declaration = messageSchema.properties[v];
            const refKey = declaration.type;
            if (!refKey) {
              console.log(messageSchema);
              throw new Error(`${filename}: ${v} has not type property. It is required`);
            }
            const ref = baseSchema.definitions[refKey];
            if (!ref) throw new Error(`${filename}: no defintion for ${refKey}`);
            required.push(v);

            return merge({}, memo, {
              definitions: {
                [refKey]: ref,
              },
              properties: {
                [v]: {
                  '$ref': `#/definitions/${refKey}`,
                  description: declaration.description || '',
                 },
              }
            });
          }, { definitions: {}, properties: {} });
      payload.required = required;

      return Object.assign({}, pick(baseSchema, ['$schema']), {
        id: `${BASE_URL}/${messageCategory}/${type}.schema.json`,
        type: 'object',
        definitions: payload.definitions,
        properties: {
          type: { type: 'string', enum: [ type ] },
          error: isException ?
              { type: 'boolean', enum: [ true ] } : undefined,
          payload: {
            type: 'object',
            properties: payload.properties,
            required: payload.required,
          },
        },
        required: isException ?
            ['payload', 'error', 'type'] : ['payload', 'type'],
        commandConstructor: messageSchema.commandConstructor,
      });
    } catch (e) {
      throw new Error(`

  Unknown Error building message from file: ${filename}
  ${e}`);

    }
  };
}
