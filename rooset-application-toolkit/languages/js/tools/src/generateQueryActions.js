const { camelCase } = require("lodash");
const {
  updateDir,
  getConfigFromEnv,
  getDeclarations,
} = require("../../../../ratk-declarations-utils");
const generateQueryRequestAction = require('./generators/generateQueryRequestAction')

const files = {};
const config = getConfigFromEnv({
  destPath: "RATK_GEN_QUERY_ACTION_JS_DEST_DIR",
  querySrcPath: "RATK_GEN_QUERY_DECL_DIR",
});
const baseSchema = require(config.baseSchemaLocation);

const queryDecls = getDeclarations(config.querySrcPath);
queryDecls.forEach(query => {
  const queryType = query.type;
  const requestContent = generateQueryRequestAction(query, config);
})
