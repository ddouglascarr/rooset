const { isArray, reduce, cloneDeep } = require('lodash');
const fs = require('fs');
const path = require('path');
const glob = require('glob');
const YAML = require('yamljs');

const testTypeEnumValues = [
 'DOMAIN_COMMAND_TEST',
 'HTTP_API_TEST',
];

module.exports = (_srcPath, testType) => {
  console.log('getTests');
  if (testTypeEnumValues.indexOf(testType) === -1) {
    throw new Error(`testType must be one of ${testTypeEnumValues}, it is ${testType}`);
  }

  const srcPath = `${_srcPath}`;
  const jsonTests = glob.sync(`${srcPath}/*.json`, { cwd: process.cwd() }) /**/
      .map((filename) => require(filename));
  const yamlTests = glob.sync(`${srcPath}/*.yaml`, { cwd: process.cwd() }) /**/
      .map((filename) => {
        try {
          return YAML.load(filename)
        } catch(err) {
          console.log(`failed to parse ${filename}`);
          console.log(err);
          throw err;
        }
      })
  console.log(yamlTests.length);
  return jsonTests.concat(yamlTests).reduce(expandScenarios, []);


  function expandScenarios(r, spec) {
    spec.scenarios = spec.scenarios.reduce((r, scenario) => {
      if (!isArray(scenario.when)) return r.concat([scenario]);
      if (!isArray(scenario.then)) throw new Error(`
          Error parsing test ${scenario.label}:
          scenario.when is an array, but scenario.then is not.`);
      const result = reduce(scenario.when, (r, whenContainer, idx) => {
        const when = whenContainer.payload;
        const then = scenario.then[idx];
        if (!isForThisTypeOfTest(testType, when, then)) return r;
        const outScenario = cloneDeep(scenario);
        outScenario.then = then;
        outScenario.when = when;
        outScenario.label = whenContainer.label;
        return r.concat([outScenario]);
      }, []);
      return r.concat(result);
    }, []);
    if (spec.scenarios.length === 0) return r;
    return r.concat([spec]);
  }
}


function isForThisTypeOfTest(testType, when, then) {
  let isDomainCommandTest = true;
  if (when.type === 'HTTP_REQUEST') isDomainCommandTest = false;
  if (then.type === 'HTTP_RESPONSE' && isDomainCommandTest) {
    throw new Error("Incompatable when and then");
  }
  return ((testType === 'DOMAIN_COMMAND_TEST') && isDomainCommandTest);
}
