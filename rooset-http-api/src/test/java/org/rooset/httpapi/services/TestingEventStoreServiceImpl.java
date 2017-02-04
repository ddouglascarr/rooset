package org.rooset.httpapi.services;

import org.apache.http.HttpHost;
import org.apache.http.HttpRequest;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.UUID;

@Service
public class TestingEventStoreServiceImpl implements TestingEventStoreService
{
  @Value("${get-event-store.host}")
  private String getEventStoreHost;

  @Value("${get-event-store.port}")
  private String getEventStorePort;



  @Override
  public void startTestingEventStore(Process process)
  {

  }



  @Override
  public void killTestingEventStore(Process process)
  {

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
