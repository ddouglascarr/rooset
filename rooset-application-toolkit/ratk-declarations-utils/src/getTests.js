const { isArray, reduce, cloneDeep } = require('lodash');
const fs = require('fs');
const glob = require('glob');
const YAML = require('yamljs');
const getDeclarations = require('./getDeclarations');

const testTypeEnumValues = [
 'DOMAIN_COMMAND_TEST',
 'HTTP_API_TEST',
];

module.exports = (_srcPath, testType) => {
  if (testTypeEnumValues.indexOf(testType) === -1) throw new Error(
      `testType ${testType} must be one of ${testTypeEnumValues}`);
      
  return getDeclarations(_srcPath).reduce(expandScenarios, []);


  function expandScenarios(r, spec) {
    spec.scenarios = spec.scenarios.reduce((r, scenario) => {
      if (!isArray(scenario.when)) {
        return isForThisTypeOfTest(testType, scenario.when, scenario.then) ?
            r.concat([scenario]) : r;
      }
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
  if (testType === 'DOMAIN_COMMAND_TEST') return isDomainCommandTest;
  return !isDomainCommandTest;
}
