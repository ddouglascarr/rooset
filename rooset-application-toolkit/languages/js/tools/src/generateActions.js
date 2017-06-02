const { camelCase } = require("lodash");
const {
  updateDir,
  getConfigFromEnv,
  getDeclarations,
} = require("ratk-declaration-utils");
const generateQueryActions = require("./generators/generateQueryActions");
const generateExecuteHttpRequest = require(
  "./generators/generateExecuteHttpRequests"
);

const files = {};
const config = getConfigFromEnv({
  destPath: "RATK_JS_QUERY_ACTION_DIR",
  querySrcPath: "RATK_DECL_QUERY_DIR",
});

const queryDecls = getDeclarations(config.querySrcPath);

const queryActionsContent = generateQueryActions(queryDecls, config);
files["QueryActions.js"] = queryActionsContent;

const executeHttpRequestContent = generateExecuteHttpRequest(
  queryDecls,
  config);
files["executeHttpRequest.js"] = executeHttpRequestContent;

updateDir(config.destPath, files);
