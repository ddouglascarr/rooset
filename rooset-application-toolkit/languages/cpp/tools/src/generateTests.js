const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
} = require('../../../../ratk-declarations-utils');

const generateTestFile = require('./generators/generateTestFile');
const generateCMakeList = require('./generators/generateCMakeList');

const config = getConfigFromEnv({
  destPath: 'RATK_GEN_TEST_CPP_DEST_DIR',
  srcPath: 'RATK_GEN_TEST_DECL_DIR',
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
