package org.rooset.httpapi.services;

import org.json.JSONObject;
import org.rooset.httpapi.exceptions.CommandEvaluationException;
import org.rooset.httpapi.exceptions.SystemException;
import org.rooset.httpapi.models.CommandServiceResponse;

public interface CommandService
{
  CommandServiceResponse execute(JSONObject command)
      throws CommandEvaluationException, SystemException;
}
