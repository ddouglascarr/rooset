const { pickBy, each, upperFirst } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('ratk-declaration-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateEnum = require('./generators/generateEnum');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_ENUM_JAVA_DEST_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};

const enumDefinitions = pickBy(baseSchema.definitions, (d) => d.type === 'string' && d.enum);
each(enumDefinitions, (enumDefinition, key) => {
  const enumClassname = upperFirst(key);
  files[`${enumClassname}.java`] =  generateEnum(
      config.javaBasePackage, enumClassname, enumDefinition.enum);
});

updateDir(config.destPath, files);
