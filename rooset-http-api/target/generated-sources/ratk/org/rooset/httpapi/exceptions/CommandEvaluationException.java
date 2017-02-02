
package org.rooset.httpapi.exceptions;

import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.enums.ExceptionType;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.json.JSONObject;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;

public class CommandEvaluationException extends RatkException
{
  public final String message;
  public final ExceptionCode exceptionCode;
  private ObjectMapper objectMapper = new ObjectMapper();
  private final ExceptionType type = ExceptionType.COMMAND_EVALUATION_EXCEPTION;

  public CommandEvaluationException(ExceptionCode exceptionCode, String message)
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
        .put("exceptionCode", exceptionCode.toString())
        .put("message", message);
    return new JSONObject()
        .put("type", type)
        .put("error", true)
        .put("payload", payload);
  }
}

