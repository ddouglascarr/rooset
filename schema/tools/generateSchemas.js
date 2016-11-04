const { merge, pick, includes } = require('lodash');
const fs = require('fs');
const glob = require('glob');
const ajv = require('ajv');
const rimfaf = require('rimraf');
const YAML = require('yamljs');
const path = require('path');

const BASE_URL = 'https://rooset.org/schemas';

const baseSchema = require('../src/base.schema.json');
const typeRe = /([A-Z_]+)/;
const isYamlFilename = (filename) => filename.slice(filename.length - 4) === 'yaml';

module.exports = (messageCategory) => {
  if (!includes(['commands', 'events', 'exceptions'], messageCategory)) {
    throw `${messageCategory} message category invalid`;
  }
  const jsonSrcFiles = glob.sync(`../src/${messageCategory}/*.json`,
      { cwd: `${__dirname}` });
  const yamlSrcFiles = glob.sync(`../src/${messageCategory}/*.yaml`,
      { cwd: __dirname });
  return jsonSrcFiles.concat(yamlSrcFiles).map(mapSchemaFiles(messageCategory));
};

/*
commandSchemas.forEach((schema) => {
  const type = typeRe.exec(schema.id)[0];
  fs.writeFileSync(`./build/commands/${type}.schema.json`, JSON.stringify(schema));
});*/


function mapSchemaFiles(destDir) {
  const isException = destDir === 'exceptions';
  return (filename) => {
    const messageSchema = isYamlFilename(filename) ?
        YAML.load(path.join(__dirname, filename)) : require(filename);
    const type = typeRe.exec(filename)[0];
    const required = [];
    const payload = Object.keys(messageSchema.properties)
        .reduce((memo, key) => {
          if (messageSchema.properties[key].slice(0,1) !== '$') {
            throw `${key} property of ${filename} must start with "$"`;
          }
          const refKey = messageSchema.properties[key].slice(1);
          const ref = baseSchema.definitions[refKey];
          if (!ref) throw `no defintion for ${refKey}`;
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
      id: `${BASE_URL}/${destDir}/${type}.schema.json`,
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
    });
  };
}
