const fs = require('fs');
const glob = require('glob');
const YAML = require('yamljs');

module.exports = (_srcPath) => {
  const srcPath = _srcPath;
  const jsonFiles = glob.sync(`${srcPath}/*.json`, { cwd: process.cwd() }) /**/
      .map((filename) => require(filename));
  const yamlFiles = glob.sync(`${srcPath}/*.yaml`, { cwd: process.cwd() }) /**/
      .map((filename) => {
        try {
          const content = fs.readFileSync(filename, "utf8");
          return YAML.parse(content);
        } catch(err) {
          console.log(`failed to parse ${filename}`);
          console.log(err);
          throw err;
        }
      })
  return jsonFiles.concat(yamlFiles);
}
