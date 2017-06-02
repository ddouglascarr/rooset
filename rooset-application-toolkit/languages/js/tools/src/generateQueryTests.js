const { camelCase } = require('lodash');
const {
  updateDir,
  getConfigFromEnv,
  getTests,
  findSchema,
  assertMessageCompliesWithSchema,
} = require('ratk-declaration-utils');
const generateQueryTest = require('./generators/generateQueryTest');


const files = {};
const config = getConfigFromEnv({
  destPath: "RATK_JS_TEST_QUERY_DIR",
  eventSrcPath: 'RATK_DECL_EVENT_DIR',
  testSrcPath: "RATK_DECL_TEST_DIR",
  querySrcPath: 'RATK_DECL_QUERY_DIR',
  reducerSrcPath: 'RATK_REDUCER_SRC_DIR',
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
