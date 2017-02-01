package org.rooset.httpapi.exceptions;

import com.fasterxml.jackson.core.JsonProcessingException;

import org.rooset.httpapi.enums.ExceptionCode;

public abstract class RatkException extends Exception
{
  public abstract String toString();
  public abstract String getMessage();
  public abstract ExceptionCode getExceptionCode();
}
