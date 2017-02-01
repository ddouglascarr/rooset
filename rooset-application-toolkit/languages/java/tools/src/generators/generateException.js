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

public class ${className} extends RatkException
{
  private String message;
  private ExceptionCode exceptionCode;
  private ObjectMapper objectMapper;

  ${className}(ExceptionCode exceptionCode, String message)
  {
    super();
    this.exceptionCode = exceptionCode;
    this.message = message;
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
  public String toString()
  {
    try {
      return objectMapper.writeValueAsString(this);
    } catch (JsonProcessingException e) {
      return "{\\"error\\": true, \\"message\\": \\"JsonProcessingException\\"}";
    }
  }
}

`;
};
