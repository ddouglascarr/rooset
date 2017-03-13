const {
  isArray,
  map,
  reduce,
  cloneDeep,
  chain,
  filter,
  groupBy,
  isObject,
  isString
} = require('lodash');
const fs = require('fs');
const path = require('path');
const glob = require('glob');
const YAML = require('yamljs');

const testTypeEnumValues = [
  'DOMAIN_COMMAND_TEST',
  'HTTP_API_TEST',
  'QUERY_TEST',
  'ALL',
];



module.exports = (_srcPath, testType, doStripMetadata = true) => {
  const allTestSpecs = {};
  if (testTypeEnumValues.indexOf(testType) === -1) {
    throw new Error(`testType must be one of ${testTypeEnumValues}, it is ${testType}`);
  }

  const srcPath = `${_srcPath}`;
  const jsonTests = glob.sync(`${srcPath}/**/*.json`, { cwd: process.cwd() })
      .map((filename) => require(filename));
  const yamlTests = glob.sync(`${srcPath}/**/*.yaml`, { cwd: process.cwd() })
      .map((filename) => {
        try {
          return YAML.load(filename)
        } catch(err) {
          console.log(`failed to parse ${filename}`);
          console.log(err.stack);
          throw new Error(err.message + filename);
        }
      })
  return chain(jsonTests.concat(yamlTests))
      .map((d) => addToAllTestSpecs(d))
      .map((d) => injectPreconditions(d))
      .map((d) => doStripMetadata ? stripMetadata(d) : d)
      .filter((d) => (testType === 'ALL' || d.type === testType))
      .value();


  function injectPreconditions(spec) {

    spec.scenarios = map(spec.scenarios, (scenario) => {
      const given = scenario.given;
      if (!isString(given)) return scenario;

      const doesRefLocalPrecondition = (given.indexOf('/') === -1);
      const [ testKey, preconditionKey] = doesRefLocalPrecondition ?
          [ spec.key, given ] : given.split('/');
      scenario.given = getPrecondition(testKey, preconditionKey);
      return scenario;
    });

    return spec;
  }


  function stripMetadata(spec) {
    spec.scenarios = spec.scenarios.map((scenario) => {
      const { given, when, then, label } = scenario;
      const out = {
        label,
        given: given.precondition ?
            given.precondition : given,
        when: when.action ? when.action : when,
        then: then.outcome ? then.outcome : then,
      };

      if (!isArray(out.given)) throw new Error(JSON.stringify(out, null, 2));

      return out;
    });
    return spec;
  }


  function addToAllTestSpecs(spec) {
    if (!spec.key) throw new Error(
        `${spec.label} does not have a key. it is required`);
    if (allTestSpecs[spec.key]) throw new Error(
        `${spec.label}: duplication spec key`);
    allTestSpecs[spec.key] = spec;
    return spec;
  }


  function getPrecondition(testKey, preconditionKey) {
    const spec = allTestSpecs[testKey];
    if (!spec) throw new Error(`
      Cannot find a test spec with key ${testKey}. Have you spelled it right?
      Should be one of these: ${JSON.stringify(Object.keys(allTestSpecs), null, 2)}`);
    let topPreconditions = {};
    if (spec.preconditions) {
      topPreconditions = reduce(spec.preconditions, (r, p) => {
        r[p.key] = p;
        return r;
      }, {});
    }
    if (!spec) throw new Error(`cannot find spec ${testKey}`);

    const preconditions = reduce(spec.scenarios, (r, scenario) => {
      if (isObject(scenario.given) && scenario.given.key) {
        r[scenario.given.key] = scenario.given;
      }
      return r;
    }, topPreconditions);

    const precondition = preconditions[preconditionKey];
    if (!precondition) throw new Error(
        `cannot find precondition ${preconditionKey} in ${testKey}`);
    return precondition;
  }


}
