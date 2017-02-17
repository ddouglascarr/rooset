const Ajv = require('ajv');
const ajv = new Ajv();


module.exports = (schema, message) => {
  const validate = ajv.compile(schema);
  const isValid = validate(message);
  if (!isValid) throw new Error(`
      Message ${message.type} does not comply with schema.
      ${ajv.errorsText()}`);
};
