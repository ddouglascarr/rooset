const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('ratk-declaration-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateCommandClass = require('./generators/generateCommand');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_COMMAND_JAVA_DEST_DIR',
  srcPath: 'RATK_GEN_COMMAND_DECL_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});

const baseSchema = require(config.baseSchemaLocation);
const files = {};

const commandSchemas = generateSchemas(baseSchema, 'commands', config.srcPath);
commandSchemas.forEach((commandSchema) => {
  const commandType = getMsgTypeFromSchema(commandSchema);
  const content = generateCommandClass(config.javaBasePackage, commandSchema);
  const filename = generateFilenameFromMsgType(commandType);
  files[`${filename}.java`] = content;
});

updateDir(config.destPath, files);
