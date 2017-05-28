const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
} = require('ratk-declaration-utils');

const generateTestFile = require('./generators/generateTestFile');
const generateCMakeList = require('./generators/generateCMakeList');

const config = getConfigFromEnv({
  destPath: 'RATK_CPP_TEST_DIR',
  srcPath: 'RATK_DECL_TEST_DIR',
});
const files = {};

// tests
const testDocs = getTests(config.srcPath, 'DOMAIN_COMMAND_TEST');
testDocs.forEach((testDoc) => {
  const content = generateTestFile(testDoc);
  const fileName = `${camelCase(testDoc.label)}.cpp`;
  files[fileName] = content;
});
files['CMakeLists.txt'] = generateCMakeList(files, 'RATK_GEN_TEST_SOURCES');

updateDir(config.destPath, files);
