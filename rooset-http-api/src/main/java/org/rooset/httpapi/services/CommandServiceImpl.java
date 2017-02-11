package org.rooset.httpapi.services;

import org.json.JSONException;
import org.json.JSONObject;
import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.enums.ExceptionType;
import org.rooset.httpapi.exceptions.CommandEvaluationException;
import org.rooset.httpapi.exceptions.SystemException;
import org.rooset.httpapi.models.CommandServiceResponse;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.UUID;

@Service
public class CommandServiceImpl implements CommandService
{

  @Value("${rooset.domain-command-bin}")
  private String domainCommandBin;

  @Value("${get-event-store.host}")
  private String getEventStoreHost;

  @Value("${get-event-store.port}")
  private String getEventStorePort;


  @Override
  public CommandServiceResponse execute(JSONObject command)
      throws CommandEvaluationException, SystemException
  {

    try {
      JSONObject resp = callDomainCommandBin(command);
      handleDomainCommandError(resp);

      return new CommandServiceResponse(
          getAggregateId(command),
          UUID.fromString(resp.getString("eventId")));

    } catch (JSONException e) {
      e.printStackTrace();
      throw new SystemException(ExceptionCode.GENERAL_PROJECT_EXCEPTION, e.getMessage());
    }
  }



  private JSONObject callDomainCommandBin(JSONObject command)
      throws SystemException, JSONException
  {
    try {
      String env[] = {
          "ROOSET_EVENT_STORE_HOST=" + getEventStoreHost,
          "ROOSET_EVENT_STORE_PORT=" + getEventStorePort};
      Process process = Runtime.getRuntime().exec(domainCommandBin, env);
      InputStream stdout = process.getInputStream();
      OutputStream stdin = process.getOutputStream();
      BufferedReader reader;

      String result = "";
      String line;
      stdin.write(command.toString().getBytes("UTF-8"));
      stdin.close();
      reader = new BufferedReader(new InputStreamReader(stdout));
      while ((line = reader.readLine()) != null) {
        result += line;
      }

      return new JSONObject(result);
    } catch (IOException e) {
      e.printStackTrace();
      throw new SystemException(ExceptionCode.GENERAL_PROJECT_EXCEPTION, e.getMessage());
    }
  }



  private UUID getAggregateId(JSONObject command)
      throws JSONException
  {
    return UUID.fromString(command.getJSONObject("payload").getString("id"));
  }



  private void handleDomainCommandError(JSONObject resp)
      throws CommandEvaluationException, SystemException, JSONException
  {
    if (resp.has("error")) {
      JSONObject payload = resp.getJSONObject("payload");
      ExceptionCode code = ExceptionCode.valueOf(payload.getString("code"));
      if (resp.getString("type").equals(ExceptionType.COMMAND_EVALUATION_EXCEPTION.toString())) {
        throw new CommandEvaluationException(
            code, payload.getString("message"));
      }
      throw new SystemException(code, payload.getString("message"));
    }
  }



}
