const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const generateMessageStatements = require('./generateMessageStatements');


module.exports = function(javaBasePackage, schema) {
  const messageType = getMsgTypeFromSchema(schema);
  const className = generateClassnameFromMsgType(messageType);
  const {
    declarations,
    stdConstructor,
  } = generateMessageStatements(schema);
  
  return `
package ${javaBasePackage}.commands;

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

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "${messageType}");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

`;
}
