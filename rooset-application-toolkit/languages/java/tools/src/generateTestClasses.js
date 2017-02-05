const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
} = require('../../../../ratk-declarations-utils');
const {
  generateFilenameFromMsgType,
  getMsgTypeFromSchema,
} = require('./utils');

const generateTestClass = require('./generators/generateTestClass');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_TEST_JAVA_DEST_DIR',
  srcPath: 'RATK_GEN_TEST_DECL_DIR',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});
const files = {};

const testDecls = getTests(config.srcPath, 'HTTP_API_TEST');
testDecls.forEach((testDecl) => {
  const fileName = camelCase(testDecl.label);
  const content = generateTestClass(config.javaBasePackage, testDecl);
  files[`${fileName}.java`] = content;
});

updateDir(config.destPath, files);
