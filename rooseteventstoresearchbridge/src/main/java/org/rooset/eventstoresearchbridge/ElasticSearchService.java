package org.rooset.eventstoresearchbridge;

import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.client.Response;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.transport.client.PreBuiltTransportClient;
import org.json.JSONObject;

import java.io.Closeable;
import java.net.InetAddress;
import java.net.UnknownHostException;

import akka.actor.ActorSystem;
import eventstore.Event;
import eventstore.SubscriptionObserver;
import eventstore.j.EsConnection;
import eventstore.j.EsConnectionFactory;

public class ElasticSearchService
{
  private TransportClient client;
  private ActorSystem system = ActorSystem.create();
  private EsConnection connection = EsConnectionFactory.create(system);

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

  public IndexResponse prepareIndex(String index, String type, String id, JSONObject body)
  {
    IndexResponse response = client.prepareIndex(index, type, id)
        .setSource(body.toString())
        .get();
    return response;
  }

  private Integer getEventStoreCheckpoint(String queryName)
  {
    return 0;
  }

  private String parseQueryName(String queryName)
  {
    return queryName.toLowerCase();
  }

  private void setEventStoreCheckpoint(String queryName, Integer eventStoreEventNumber)
  {
    client.prepareIndex(parseQueryName(queryName), "checkpoint", "value")
        .setSource(new JSONObject().put("eventStoreEventNumber", eventStoreEventNumber).toString())
        .get();
  }

  public Closeable subscribeToQueryStream(final String queryName)
  {
    return connection.subscribeToStreamFrom("$projections-" + queryName + "-result", new SubscriptionObserver<Event>()
    {
      public void onLiveProcessingStart(Closeable closeable)
      {
        system.log().info("live processing started");
      }

      public void onEvent(Event e, Closeable closeable)
      {
        JSONObject json = new JSONObject(e.data().data().value().decodeString("utf8"));
        final String id = json.getString("id");
        IndexResponse resp = prepareIndex(parseQueryName(queryName), "state", id, json);
        system.log().info(new Boolean(resp.status().toString() == "OK").toString());
        system.log().info("finished " + e.data().eventId());
        setEventStoreCheckpoint(queryName, e.number().value());
      }

      public void onError(Throwable err)
      {
        system.log().error(err.toString());

      }

      public void onClose()
      {
        system.log().info("ElasticSearch  connection closed");
      }
    }, getEventStoreCheckpoint(queryName), true, null);
  }

  public void close() {
    client.close();
  }
}
