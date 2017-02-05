const {
  generateClassnameFromMsgType,
} = require('../utils');

module.exports = function(javaBasePackage, exceptionType) {
  const className = generateClassnameFromMsgType(exceptionType);

  return `
package ${javaBasePackage}.exceptions;

import ${javaBasePackage}.exceptions.RatkException;
import ${javaBasePackage}.enums.ExceptionCode;
import ${javaBasePackage}.enums.ExceptionType;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.json.JSONObject;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;

public class ${className} extends RatkException
{
  public final String message;
  public final ExceptionCode exceptionCode;
  private ObjectMapper objectMapper = new ObjectMapper();
  private final ExceptionType type = ExceptionType.${exceptionType};

  public ${className}(ExceptionCode exceptionCode, String message)
  {
    this.exceptionCode = exceptionCode;
    this.message = message;
    
    objectMapper
        .registerModule(new JsonOrgModule())
        .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
  }

  @Override
  public ExceptionCode getExceptionCode()
  {
    return exceptionCode;
  }

  @Override
  public String getMessage()
  {
    return message;
  }

  @Override
  public JSONObject serialize()
  {
    JSONObject payload = new JSONObject()
        .put("code", exceptionCode.toString())
        .put("message", message);
    return new JSONObject()
        .put("type", type)
        .put("error", true)
        .put("payload", payload);
  }
}

`;
};
