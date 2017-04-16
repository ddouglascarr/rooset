package org.rooset.eventstoresearchbridge;

import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.transport.client.PreBuiltTransportClient;
import org.json.JSONObject;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class ElasticSearchService
{
  private TransportClient client;

  public ElasticSearchService()
  {
    try {
      client = new PreBuiltTransportClient(Settings.EMPTY)
          .addTransportAddress(new InetSocketTransportAddress(
              InetAddress.getByName("localhost"), 9300));
    } catch (UnknownHostException e) {
      e.printStackTrace();
    }
  }


  public void prepareIndex(String index, String type, String id, JSONObject body)
  {
    client.prepareIndex(index, type, id)
        .setSource(body.toString())
        .get();
  }

  public void close() {
    client.close();
  }
}
