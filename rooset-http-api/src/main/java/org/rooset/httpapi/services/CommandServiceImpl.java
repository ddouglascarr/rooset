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


  @Override
  public CommandServiceResponse execute(JSONObject command)
      throws CommandEvaluationException, SystemException
  {

    try {
      String env[] = {"ROOSET_EVENT_STORE_HOST=localhost", "ROOSET_EVENT_STORE_PORT=2113"};
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

      JSONObject resp = new JSONObject(result);

      if (resp.has("error")) {
        JSONObject payload = resp.getJSONObject("payload");
        ExceptionCode code = ExceptionCode.valueOf(payload.getString("code"));
        if (resp.getString("type").equals(ExceptionType.COMMAND_EVALUATION_EXCEPTION.toString())) {
          throw new CommandEvaluationException(
              code, payload.getString("message"));
        }
        throw new SystemException(code, payload.getString("message"));
      }

      return new CommandServiceResponse(UUID.fromString(resp.getString("eventId")));

    } catch(IOException e) {
      throw new SystemException(ExceptionCode.GENERAL_PROJECT_EXCEPTION, e.getMessage());
    } catch (JSONException e) {
      throw new SystemException(ExceptionCode.GENERAL_PROJECT_EXCEPTION, e.getMessage());
    }
  }



}
