
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
  queryControllerDestPath: 'RATK_GEN_HTTP_QUERY_CONTROLLER_JAVA_DEST_DIR',
  srcPath: 'RATK_GEN_QUERY_DECL_DIR',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});

const files = {};
const generateHttpQueryRequestController = require(
    './generators/generateHttpQueryRequestController');

const queryDeclarations = getDeclarations(config.srcPath);
queryDeclarations.forEach((declaration) => {
  const fileName = `${generateFilenameFromMsgType(declaration.type)}HttpQueryController`;
  const content = generateHttpQueryRequestController(declaration, config);
  files[`${fileName}.java`] = content;
});

updateDir(config.queryControllerDestPath, files);
