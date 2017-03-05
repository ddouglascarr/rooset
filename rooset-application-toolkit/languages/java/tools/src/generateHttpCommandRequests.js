const { find } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  generateSchemas,
  getDeclarations,
  findCommandSchemaByType,
} = require('ratk-declaration-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const config = getConfigFromEnv({
  controllerDestPath: 'RATK_GEN_HTTP_CONTROLLER_JAVA_DIR',
  requestBodyDestPath: 'RATK_GEN_HTTP_REQUEST_BODY_JAVA_DIR',
  srcPath: 'RATK_GEN_HTTP_COMMAND_REQUEST_DECL_DIR',
  commandSrcPath: 'RATK_GEN_COMMAND_DECL_DIR',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
});

const baseSchema = require(config.baseSchemaLocation);
const controllerFiles = {};
const requestBodyFiles = {};

const generateHttpCommandRequestBody = require('./generators/generateHttpCommandRequestBody');
const generateHttpCommandRequestController = require(
    './generators/generateHttpCommandRequestController');

const requestDeclarations = getDeclarations(config.srcPath);
requestDeclarations.forEach((declaration) => {
  const commandSchema = findCommandSchemaByType(declaration.commandTarget);

  const reqBodyContent = generateHttpCommandRequestBody(
      config.javaBasePackage, declaration, commandSchema);
  const reqBodyFilename = generateFilenameFromMsgType(declaration.type);
  requestBodyFiles[`${reqBodyFilename}Body.java`] = reqBodyContent;

  const controllerContent = generateHttpCommandRequestController(
      config.javaBasePackage, declaration, commandSchema);
  const controllerFilename = generateFilenameFromMsgType(`${declaration.type}_CONTROLLER`);
  controllerFiles[`${controllerFilename}.java`] = controllerContent;
});

updateDir(config.requestBodyDestPath, requestBodyFiles);
updateDir(config.controllerDestPath, controllerFiles);
