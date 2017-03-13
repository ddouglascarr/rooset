package org.rooset.httpapi.services;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DaemonExecutor;
import org.apache.commons.exec.DefaultExecuteResultHandler;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.ProcessDestroyer;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.http.HttpStatus;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Profile;
import org.springframework.stereotype.Service;
import org.springframework.test.context.ActiveProfiles;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import static java.lang.Thread.sleep;

@Service
@Profile("integration-tests")
public class TestingEventStoreServiceImpl implements TestingEventStoreService
{
  @Value("${get-event-store.host}")
  private String getEventStoreHost;

  @Value("${get-event-store.port}")
  private String getEventStorePort;

  @Value("${get-event-store.binPath}")
  private String getEventStoreBinPath;

  @Value("${get-event-store.init-all-projections-bin}")
  private String initAllProjectionsBin;

  @Override
  public ExecuteWatchdog startTestingEventStore()
      throws IOException
  {
    final ByteArrayOutputStream bos = new ByteArrayOutputStream();
    final PumpStreamHandler psh = new PumpStreamHandler(bos);

    CommandLine cmdLine = new CommandLine("eventstored");
    cmdLine.addArgument("--mem-db");
    cmdLine.addArgument("--ext-http-port");
    cmdLine.addArgument(getEventStorePort);
    cmdLine.addArgument("--run-projections=all");

    DefaultExecuteResultHandler resultHandler = new DefaultExecuteResultHandler();
    final DaemonExecutor executor = new DaemonExecutor();
    ExecuteWatchdog watchdog = new ExecuteWatchdog(60000);
    executor.setStreamHandler(psh);
    executor.setWatchdog(watchdog);
    // executor.setWorkingDirectory(new File(getEventStoreBinPath));
    Map<String, String> env = new HashMap<>();
    executor.execute(cmdLine, resultHandler);

    waitForHttpServer(resultHandler, bos);

    return watchdog;
  }


  private void waitForHttpServer(
      final DefaultExecuteResultHandler resultHandler,
      final ByteArrayOutputStream bos) throws IOException
  {

    String upMessage = "Sub System 'Projections' initialized";
    int sleepMs = 200;

    try {
      while (!resultHandler.hasResult()
          && !bos.toString().contains(upMessage)) {
        sleep(sleepMs);
      }
    } catch (InterruptedException e) {
      e.printStackTrace();
      throw new IOException("sleep failed");
    }

    if (resultHandler.hasResult()) {
      System.out.println(bos.toString());
      throw new IOException("GetEventStore exited with code: " + resultHandler.getExitValue());
    }

    if (bos.toString().contains(upMessage)) {
      // Success
      return;
    }

    throw new IOException("Timed out waiting for GetEentStore to start");
  }


  @Override
  public void killTestingEventStore(Process process)
      throws IOException
  {
    process.destroy();
  }



  @Override
  public JSONObject getLastEventForAggregate(UUID aggregateId)
      throws IOException
  {
    JSONObject aggregateStream = getAggregateStream(aggregateId);
    JSONObject lastEntry = aggregateStream.getJSONArray("entries").getJSONObject(0);
    String eventUri = lastEntry.getJSONArray("links").getJSONObject(0).getString("uri");
    return getEvent(eventUri);
  }



  private JSONObject getAggregateStream(UUID aggregateId)
      throws IOException
  {
    CloseableHttpClient httpClient = HttpClients.custom().build();
    try {
      HttpGet get = new HttpGet("http://" + getEventStoreHost + ":" + getEventStorePort +
          "/streams/aggregate-" + aggregateId.toString());
      get.addHeader("Accept", "application/vnd.eventstore.atom+json");
      CloseableHttpResponse resp = httpClient.execute(get);
      String respBodyStr = EntityUtils.toString(resp.getEntity());
      return new JSONObject(respBodyStr);
    } finally {
      httpClient.close();
    }
  }



  private JSONObject getEvent(String uri)
      throws IOException
  {
    CloseableHttpClient httpClient = HttpClients.custom().build();
    try {
      HttpGet get = new HttpGet(uri);
      get.addHeader("Accept", "application/json");
      CloseableHttpResponse resp = httpClient.execute(get);
      String respBodyStr = EntityUtils.toString(resp.getEntity());
      return new JSONObject(respBodyStr);
    } finally {
      httpClient.close();
    }
  }


  @Override
  public void saveEvent(JSONObject event) throws Exception
  {
    String aggregateId = event.getJSONObject("payload").getString("id");
    String eventType = event.getString("type");
    CloseableHttpClient httpClient = HttpClients.custom().build();
    try {
      HttpPost post = new HttpPost("http://" + getEventStoreHost + ":" + getEventStorePort +
          "/streams/aggregate-" + aggregateId);
      post.addHeader("Content-Type", "application/json");
      post.addHeader("ES-EventType", eventType);
      post.addHeader("ES-EventId", UUID.randomUUID().toString());
      post.setEntity(new ByteArrayEntity(event.toString().getBytes("UTF-8")));
      CloseableHttpResponse resp = httpClient.execute(post);
      if (201 != resp.getStatusLine().getStatusCode()) {
        System.out.println(resp.getEntity().getContent().toString());
        throw new IOException("Could not create Event: \n" +
            resp.getStatusLine() + "\n" +
            resp.getEntity().getContent().toString());
      }
    } finally {
      httpClient.close();
    }
  }

  @Override
  public void initProjections() throws Exception
  {
    CommandLine cmd = new CommandLine(initAllProjectionsBin);
    final DefaultExecutor executor = new DefaultExecutor();
    int returnValue = executor.execute(cmd);
    if (returnValue != 0) {
      throw new Exception("init-all-projections returned code: " + returnValue);
    }
  }
}
