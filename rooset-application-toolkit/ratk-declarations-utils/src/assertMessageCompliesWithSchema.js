const Ajv = require('ajv');
const ajv = new Ajv({
  allErrors: true,
  verbose: true,
  format: 'full',
  validateSchema: 'log',
});


module.exports = (schema, message) => {
  try {
    const validate = ajv.compile(schema);
    const isValid = validate(message);
    if (!isValid) throw new Error(`
        Message ${message.type} does not comply with schema.
        ${ajv.errorsText()}`);
  } catch (e) {
    console.log(e.stack);
    throw new Error(`
      Message ${message.type} has invalid schema.
      ${e}
${JSON.stringify(schema, null, 2)}
${JSON.stringify(message, null, 2)}`);
  }
};
