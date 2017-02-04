package org.rooset.httpapi.services;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DaemonExecutor;
import org.apache.commons.exec.DefaultExecuteResultHandler;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.ProcessDestroyer;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import static java.lang.Thread.sleep;

@Service
public class TestingEventStoreServiceImpl implements TestingEventStoreService
{
  @Value("${get-event-store.host}")
  private String getEventStoreHost;

  @Value("${get-event-store.port}")
  private String getEventStorePort;

  @Value("${get-event-store.bin}")
  private String getEventStoreBin;

  @Value("${get-event-store.binPath}")
  private String getEventStoreBinPath;


  @Override
  public ExecuteWatchdog startTestingEventStore()
      throws IOException
  {
    final ByteArrayOutputStream bos = new ByteArrayOutputStream();
    final PumpStreamHandler psh = new PumpStreamHandler(bos);

    String LD_LIBRARY_PATH = System.getenv("LD_LIBRARY_PATH");
    String cmd[] = {"./eventstored", "--mem-db", "--ext-http-port", getEventStorePort};

    CommandLine cmdLine = new CommandLine("./run-node.sh");
    cmdLine.addArgument("--mem-db");
    cmdLine.addArgument("--ext-http-port");
    cmdLine.addArgument(getEventStorePort);

    DefaultExecuteResultHandler resultHandler = new DefaultExecuteResultHandler();
    final DaemonExecutor executor = new DaemonExecutor();
    ExecuteWatchdog watchdog = new ExecuteWatchdog(60000);
    executor.setStreamHandler(psh);
    executor.setWatchdog(watchdog);
    executor.setWorkingDirectory(new File(getEventStoreBinPath));
    Map<String, String> env = new HashMap<>();
    executor.execute(cmdLine, resultHandler);

    waitForHttpServer(resultHandler, bos);

    return watchdog;
  }


  private void waitForHttpServer(
      final DefaultExecuteResultHandler resultHandler,
      final ByteArrayOutputStream bos) throws IOException
  {

    String upMessage = "IS MASTER";
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
          "/streams/" + aggregateId.toString());
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



}
