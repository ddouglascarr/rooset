const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
} = require('../../../../ratk-declarations-utils');
const generateQueryTestClass = require('./generators/generateQueryTestClass');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_QUERY_TEST_JAVA_DEST_DIR',
  srcPath: 'RATK_GEN_TEST_DECL_DIR',
  queryDeclDir: 'RATK_GEN_QUERY_DECL_DIR',
  javaBasePackage: 'RATK_GEN_JAVA_BASE_PACKAGE',
});

const files = {};
const testDecls = getTests(config.srcPath, 'QUERY_TEST', false);
testDecls.forEach((testDecl) => {
  const fileName = camelCase(testDecl.label);
  const content = generateQueryTestClass(testDecl, config);
  files[`${fileName}.java`] = content;
});

updateDir(config.destPath, files);
