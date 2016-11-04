const fs = require('fs');
const path = require('path');
const glob = require('glob');
const YAML = require('yamljs');

module.exports = () => {
  const jsonTests = glob.sync('../src/tests/*.json', { cwd: `${__dirname}`})
      .map((filename) => require(filename));
  const yamlTests = glob.sync('../src/tests/*.yaml', { cwd: `${__dirname}` })
      .map((filename) => YAML.load(path.join(__dirname, filename)))
  return jsonTests.concat(yamlTests);
}
