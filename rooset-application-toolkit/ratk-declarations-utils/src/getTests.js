const fs = require('fs');
const path = require('path');
const glob = require('glob');
const YAML = require('yamljs');

module.exports = (_srcPath) => {
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
  return jsonTests.concat(yamlTests);
}
