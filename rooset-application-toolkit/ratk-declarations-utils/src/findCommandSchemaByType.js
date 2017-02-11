const { find } = require('lodash');
const getDeclarations = require('./generateSchemas');
const getConfigFromEnv = require('./getConfigFromEnv');
const getMsgTypeFromSchema = (schema) => schema.properties.type.enum[0];

let commandSchemas = null;



module.exports = (commandType) => {
  const config = getConfigFromEnv({
    commandSrcPath: 'RATK_GEN_COMMAND_DECL_DIR',
    baseSchemaLocation: 'RATK_GEN_BASE_SCHEMA_FILE',
  });
  const baseSchema = require(config.baseSchemaLocation);

  if (commandSchemas === null) {
    commandSchemas = getDeclarations(baseSchema, 'commands', config.commandSrcPath);
  }

  const commandSchema = find(commandSchemas,
      (s) => getMsgTypeFromSchema(s) === commandType);
  if (!commandSchema) throw new Error(`
      could not find command schema: ${commandType}`);
  return commandSchema;
}
