const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('../../../../ratk-declarations-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateExceptionClass = require('./generators/generateException');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_EXCEPTION_JAVA_DEST_DIR',
  srcPath: 'RATK_GEN_EXCEPTION_DECL_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};

const exceptionSchemas = generateSchemas(baseSchema, 'exceptions', config.srcPath);
exceptionSchemas.forEach((exceptionSchema) => {
  const exceptionType = getMsgTypeFromSchema(exceptionSchema);
  const content = generateExceptionClass(config.javaBasePackage, exceptionType);
  const fileName = generateFilenameFromMsgType(exceptionType);
  files[`${fileName}.java`] = content;
});

updateDir(config.destPath, files);
