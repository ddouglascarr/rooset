const { find } = require('lodash');
const getConfigFromEnv = require('./getConfigFromEnv');
const getTests = require('./getTests');


module.exports = (testKey, scenarioKey) => {
  const config = getConfigFromEnv({
    testSrcPath: 'RATK_GEN_TEST_DECL_DIR',
  });

  const testSpecs = getTests(config.testSrcPath, 'ALL', false);
  const testSpec = find(testSpecs, (spec) => spec.key === testKey);
  if (!testSpec) throw new Error(`
      Could not find spec with key ${testKey}`);
  const scenario = find(testSpec.scenarios,
      (scenario) => scenario.given.key === scenarioKey);
  if (!scenario) throw new Error(`
      Could not find scenario with key ${scenarioKey} in test with key ${testKey}`);

  const events = scenario.given.precondition;
  return events;
};
