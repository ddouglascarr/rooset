const { cloneDeep, omit }  = require('lodash');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const {
  getPathVariables,
} = require('../../../../../ratk-declarations-utils');
const generateMessageStatements = require('./generateMessageStatements');

module.exports = function(javaBasePackage, declaration, commandSchema) {
  const messageType = `${declaration.type}_BODY`;
  const className = generateClassnameFromMsgType(messageType);
  const commandMessageType = getMsgTypeFromSchema(commandSchema);
  const commandClassName = generateClassnameFromMsgType(commandMessageType);

  const omittedPayloadProps = [];
  if (declaration.generate) {
    declaration.generate.forEach((prop) => omittedPayloadProps.push(prop));
  }
  if (declaration.userIdMapping) omittedPayloadProps.push(declaration.userIdMapping);
  getPathVariables(declaration.uri).forEach((prop) => omittedPayloadProps.push(prop));

  const requestBodySchema = cloneDeep(commandSchema);
  requestBodySchema.properties.type.enum = [messageType];
  requestBodySchema.properties.payload.properties = omit(
      commandSchema.properties.payload.properties, omittedPayloadProps);
  const {
    declarations,
    stdConstructor,
  } = generateMessageStatements(requestBodySchema);

  return `
package ${javaBasePackage}.httpcommandrequestbodies;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class ${className}
{
  private ObjectMapper objectMapper = new ObjectMapper();

  ${declarations.join('\n  ')}

  @JsonCreator
  ${stdConstructor}

}

`;


};
