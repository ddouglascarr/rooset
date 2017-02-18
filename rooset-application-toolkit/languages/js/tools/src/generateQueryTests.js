const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
  findSchema,
  assertMessageCompliesWithSchema,
} = require('../../../../ratk-declarations-utils');
const generateQueryTest = require('./generators/generateQueryTest');


const files = {};
const config = getConfigFromEnv({
  destPath: "RATK_GEN_TEST_QUERY_JS_DEST_DIR",
  eventSrcPath: 'RATK_GEN_EVENT_DECL_DIR',
  testSrcPath: "RATK_GEN_TEST_DECL_DIR",
  querySrcPath: 'RATK_GEN_QUERY_DECL_DIR',
  reducerSrcPath: 'RATK_GEN_REDUCER_SRC_DIR',
  baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
});
const baseSchema = require(config.baseSchemaLocation);

const testDecls = getTests(config.testSrcPath, 'QUERY_TEST', false);
testDecls.forEach((testDecl) => {
  validateTestDecl(testDecl);
  const fileName = camelCase(testDecl.label);
  const content = generateQueryTest(
      testDecl, config.querySrcPath, config.reducerSrcPath);
  files[`${fileName}.spec.js`] = content;
});

updateDir(config.destPath, files);

function validateTestDecl(testDecl) {
  testDecl.scenarios.forEach((scenario) => {
    scenario.given.precondition.forEach((event) => {
      const type = event.type;
      if (!type) throw new Error(
          `No type for event: ${event} in ${scenario.label}`);
      const schema = findSchema(baseSchema, "events", config.eventSrcPath, type);
      assertMessageCompliesWithSchema(schema, event);
    });
  });
}
