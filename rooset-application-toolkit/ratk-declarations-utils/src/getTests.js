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
 'ALL',
];



module.exports = (_srcPath, testType) => {
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
          console.log(err);
          throw err;
        }
      })
  return chain(jsonTests.concat(yamlTests))
      .map((d) => addToAllTestSpecs(d))
      .map((d) => injectPreconditions(d))
      .map((d) => stripMetadata(d))
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
    if (spec.key === 'unit-aggregate-command') console.log(
        JSON.stringify(spec, null, 2));
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

      /*
      if (scenario.given.precondition) console.log(JSON.stringify(scenario, null, 2));
      scenario.given = scenario.given.precondition ?
          scenario.given.precondition : scenario.given;
      if (scenario.when.action) scenario.when = scenario.when.action;
      if (scenario.then.outcome) scenario.then = scenario.then.outcome;
      if (!isArray(scenario.given)) throw new Error(JSON.stringify(scenario, null, 2));
      return scenario;*/
    });
    if (spec.key === 'unit-aggregate-command') console.log(
        JSON.stringify(spec, null, 2));
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
        if (r[scenario.given.key]) {
          console.log(r);
          console.log(scenario);
          console.log(scenario.given.key);
        }
        r[scenario.given.key] = scenario.given;
      }
      return r;
    }, topPreconditions);

    const precondition = preconditions[preconditionKey];
    if (!precondition) throw new Error(
        `cannot find precondition ${preconditionKey} in ${testKey}`);
    return precondition;
  }


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
