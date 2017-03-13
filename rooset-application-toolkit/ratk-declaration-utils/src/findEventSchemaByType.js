const { find } = require("lodash");
const generateSchemas = require("./generateSchemas");
const getConfigFromEnv = require("./getConfigFromEnv");
const getMsgTypeFromSchema = schema => schema.properties.type.enum[0];

let eventSchemas = null;

module.exports = eventType => {
  const config = getConfigFromEnv({
    eventSrcPath: "RATK_GEN_EVENT_DECL_DIR",
    baseSchemaLocation: "RATK_GEN_BASE_SCHEMA_FILE",
  });
  const baseSchema = require(config.baseSchemaLocation);
  if (eventSchemas === null) {
    eventSchemas = generateSchemas(baseSchema, "events", config.eventSrcPath);
  }

  const eventSchema = find(
    eventSchemas,
    s => getMsgTypeFromSchema(s) === eventType
  );
  if (!eventSchema)
    throw new Error(
      `
      could not find event schema ${eventType}`
    );

  return eventSchema;
};
