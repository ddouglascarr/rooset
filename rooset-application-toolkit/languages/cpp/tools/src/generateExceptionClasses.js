const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('ratk-declaration-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateExceptionClass = require('./generators/generateExceptionClass');
const generateMessageCpp = require('./generators/generateMessageCpp');
const generateCMakeList = require('./generators/generateCMakeList');

const config = getConfigFromEnv({
  destPath: 'RATK_CPP_EXCEPTION_DIR',
  srcPath: 'RATK_DECL_EXCEPTION_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};

const exceptionSchemas = generateSchemas(baseSchema, 'exceptions', config.srcPath);
exceptionSchemas.forEach((exceptionSchema) => {
  const exceptionType = getMsgTypeFromSchema(exceptionSchema);
  const content = generateExceptionClass(exceptionType);
  const fileName = generateFilenameFromMsgType(exceptionType);
  files[`${fileName}.h`] = content;
  const cppContent = generateMessageCpp(exceptionSchema, 'exceptions');
  files[`${fileName}.cpp`] = cppContent;
});
files['CMakeLists.txt'] = generateCMakeList(files, 'RATK_GEN_EXCEPTION_SOURCES');

updateDir(config.destPath, files);
