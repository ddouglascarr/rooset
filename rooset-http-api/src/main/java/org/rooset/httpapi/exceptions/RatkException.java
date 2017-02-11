package org.rooset.httpapi.exceptions;

import com.fasterxml.jackson.core.JsonProcessingException;
import org.json.JSONObject;

import org.rooset.httpapi.enums.ExceptionCode;

public abstract class RatkException extends Exception
{
  public abstract JSONObject serialize();
  public abstract String getMessage();
  public abstract ExceptionCode getExceptionCode();
}
