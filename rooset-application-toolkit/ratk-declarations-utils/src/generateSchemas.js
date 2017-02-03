const { merge, pick, includes } = require('lodash');
const fs = require('fs');
const glob = require('glob');
const ajv = require('ajv');
const YAML = require('yamljs');
const path = require('path');

const BASE_URL = 'https://rooset.org/schemas';

const typeRe = /([A-Z_]+)/;
const isYamlFilename = (filename) => filename.slice(filename.length - 4) === 'yaml';


module.exports = (baseSchema, messageCategory, _srcPath) => {
  const srcPath = `${_srcPath}`;
  if (!includes(['commands', 'events', 'exceptions'], messageCategory)) {
    throw `${messageCategory} message category invalid`;
  }
  const jsonSrcFiles = glob.sync(`${srcPath}/*.json`, /**/
      { cwd: process.cwd() });
  const yamlSrcFiles = glob.sync(`${srcPath}/*.yaml`, /**/
      { cwd: process.cwd() });
  return jsonSrcFiles.concat(yamlSrcFiles).map(mapSchemaFiles(baseSchema, messageCategory));
};


function mapSchemaFiles(baseSchema, messageCategory) {
  const isException = messageCategory === 'exceptions';
  return (filename) => {
    const messageSchema = isYamlFilename(filename) ?
        YAML.load(filename) : require(filename);
    const type = typeRe.exec(filename)[0];
    const required = [];
    const payload = Object.keys(messageSchema.properties)
        .reduce((memo, v) => {
          const declaration = messageSchema.properties[v];
          const refKey = declaration.type;
          if (!refKey) throw new Error(`${v} has not type property. It is required`);
          const ref = baseSchema.definitions[refKey];
          if (!ref) throw new Error(`no defintion for ${refKey}`);
          required.push(key);

          return merge({}, memo, {
            definitions: {
              [refKey]: ref,
            },
            properties: {
              [key]: { '$ref': `#/definitions/${refKey}` },
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
  };
}
