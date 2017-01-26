const { each, isUndefined } = require('lodash');

module.exports = (envMap) => {
  const config = {};
  each(envMap, (envKey, configKey) => {
    const val = process.env[envKey];
    if (isUndefined(val)) throw Error(`
${envKey} is not set, these environment variables are required:
${Object.keys(envMap).map((k) => envMap[k]).join('\n')}`);

    config[configKey] = val;
  });
  return config;
};
