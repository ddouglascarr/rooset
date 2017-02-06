const { find } = require('lodash');
const getDeclarations = require('./getDeclarations');
const getConfigFromEnv = require('./getConfigFromEnv');
const matchUri = require('./matchUri');

let requestDeclarations = null;

module.exports = (uri) => {
  const config = getConfigFromEnv({
    srcPath: 'RATK_GEN_HTTP_COMMAND_REQUEST_DECL_DIR',
  });

  if (requestDeclarations === null) {
    requestDeclarations = getDeclarations(config.srcPath);
  }

  const request = find(requestDeclarations,
      (r) => matchUri(r.uri, uri));
  if (!request) throw new Error(`
    could not find HttpCommandRequest with uri of ${uri}`);
  return request;
};
