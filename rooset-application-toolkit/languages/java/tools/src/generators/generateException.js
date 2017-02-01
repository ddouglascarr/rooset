const {
  generateClassnameFromMsgType,
} = require('../utils');

module.exports = function(javaBasePackage, exceptionType) {
  const className = generateClassnameFromMsgType(exceptionType);

  return `
package ${javaBasePackage}.exceptions;

import ${javaBasePackage}.exceptions.RatkException;
import ${javaBasePackage}.enums.ExceptionCode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.json.JSONObject;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;

public class ${className} extends RatkException
{
  private String message;
  private ExceptionCode exceptionCode;
  private ObjectMapper objectMapper = new ObjectMapper();

  public ${className}(ExceptionCode exceptionCode, String message)
  {
    this.exceptionCode = exceptionCode;
    this.message = message;
    
    objectMapper.registerModule(new JsonOrgModule());
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
    return objectMapper.convertValue(this, JSONObject.class);
  }
}

`;
};
