const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('ratk-declaration-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateCommandClass = require('./generators/generateCommandClass');
const generateMessageCpp = require('./generators/generateMessageCpp');
const generateCommandUtils = require('./generators/generateCommandUtils');
const generateCMakeList = require('./generators/generateCMakeList');


const config = getConfigFromEnv({
  destPath: 'RATK_CPP_COMMAND_DIR',
  srcPath: 'RATK_DECL_COMMAND_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};

const commandSchemas = generateSchemas(baseSchema, 'commands', config.srcPath);
console.log('commandSchemas length' + commandSchemas.length);
commandSchemas.forEach((commandSchema) => {
  const content = generateCommandClass(commandSchema);
  const msgType = getMsgTypeFromSchema(commandSchema);
  const fileName = generateFilenameFromMsgType(msgType);
  files[`${fileName}.h`] = content;
  const cppContent = generateMessageCpp(commandSchema, 'commands');
  files[`${fileName}.cpp`] = cppContent;
});
files['CommandUtils.h'] = generateCommandUtils(commandSchemas);
files['CMakeLists.txt'] = generateCMakeList(files, 'RATK_GEN_COMMAND_SOURCES');

updateDir(config.destPath, files);

