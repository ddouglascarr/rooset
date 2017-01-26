const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
} = require('../../../../ratk-declarations-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');
const { find } = require('lodash');

const generateEventClass = require('./generators/generateEventClass');
const generateMessageCpp = require('./generators/generateMessageCpp');
const generateEventUtils = require('./generators/generateEventUtils');
const generateCMakeList = require('./generators/generateCMakeList');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_EVENT_CPP_DEST_DIR',
  srcPath: 'RATK_GEN_EVENT_DECL_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
  cmdSrcPath: 'RATK_GEN_COMMAND_DECL_DIR',
});
const baseSchema = require(config.baseSchemaLocation);
const files = {};

const commandSchemas = generateSchemas(baseSchema, 'commands', config.cmdSrcPath);
const eventSchemas = generateSchemas(baseSchema, 'events', config.srcPath);
eventSchemas.forEach((eventSchema) => {
  let commandConstructorSchema = null;
  if (eventSchema.commandConstructor) {
    commandConstructorSchema = find(commandSchemas,
        (s) => getMsgTypeFromSchema(s) === eventSchema.commandConstructor);
    if (!commandConstructorSchema) {
      throw `no commandSchema found: ${eventSchema.commandConstructor}`;
    }
  }
  const content = generateEventClass(eventSchema, commandConstructorSchema);
  const msgType = getMsgTypeFromSchema(eventSchema);
  const fileName = generateFilenameFromMsgType(msgType);
  files[`${fileName}.h`] = content;
  const cppContent = generateMessageCpp(eventSchema, 'events');
  files[`${fileName}.cpp`] = cppContent;
});
files['EventUtils.h'] = generateEventUtils(eventSchemas);
files['CMakeLists.txt'] = generateCMakeList(files, 'RATK_GEN_EVENT_SOURCES');

updateDir(config.destPath, files);
