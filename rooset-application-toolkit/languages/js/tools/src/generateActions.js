const { camelCase } = require("lodash");
const {
  updateDir,
  getConfigFromEnv,
  getDeclarations,
} = require("../../../../ratk-declarations-utils");
const generateQueryActions = require('./generators/generateQueryActions')

const files = {};
const config = getConfigFromEnv({
  destPath: "RATK_GEN_QUERY_ACTION_JS_DEST_DIR",
  querySrcPath: "RATK_GEN_QUERY_DECL_DIR",
});

const queryDecls = getDeclarations(config.querySrcPath);
const queryActionsContent = generateQueryActions(queryDecls, config);
files["QueryActions.js"] = queryActionsContent;

updateDir(config.destPath, files);
