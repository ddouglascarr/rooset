package org.rooset.eventstoresearchbridge;

import org.apache.lucene.index.IndexNotFoundException;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.transport.client.PreBuiltTransportClient;
import org.json.JSONException;
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

  private String buildElasticSearchIndexName(final String index, final String indexBy, JSONObject body)
  {
    String elasticSearchIndexName = index;
    if (null != indexBy) {
      elasticSearchIndexName = index + "-" + body.getString(indexBy);
    }
    return elasticSearchIndexName;
  }

  private String buildElasticSearchCheckpointId(final String index, final String indexBy, final String type)
  {
    if (null != indexBy) {
      return index + "-" + indexBy + "-" + type;
    }
    return index + "-" + type;
  }

  public IndexResponse prepareIndex(String index, String type, String id, JSONObject body)
  {
    IndexResponse response = client.prepareIndex(index, type, id)
        .setSource(body.toString())
        .get();
    return response;
  }

  private Integer getEventStoreCheckpoint(String index, String indexBy, String type)
  {
    if (!doesIndexExist("rooset-checkpoints")) return 0;

    GetResponse resp = client
        .prepareGet("rooset-checkpoints", "checkpoint", buildElasticSearchCheckpointId(index, indexBy, type))
        .get();
    if(resp.isSourceEmpty()) return 0;

    JSONObject result = new JSONObject(resp.getSourceAsString());
    Integer checkpoint = result.getInt("eventStoreEventNumber");
    system.log().info("checkpoint: " + checkpoint);
    return checkpoint;
  }

  private void setEventStoreCheckpoint(String index, String indexBy, String type, Integer eventStoreEventNumber)
  {
    client.prepareIndex("rooset-checkpoints", "checkpoint", buildElasticSearchCheckpointId(index, indexBy, type))
        .setSource(new JSONObject().put("eventStoreEventNumber", eventStoreEventNumber).toString())
        .get();
  }

  private boolean doesIndexExist(String index)
  {
    return client.admin().indices()
        .prepareExists(index)
        .execute().actionGet().isExists();
  }

  public Closeable subscribeToQueryStream(final String query, final String typeName, final String indexBy, final String indexName)
  {
    system.log().info("subscribing to " + query);
    return connection.subscribeToStreamFrom("$projections-" + query + "-result", new SubscriptionObserver<Event>()
    {
      public void onLiveProcessingStart(Closeable closeable)
      {
        System.out.println("LIVE_PROCESSING_STARTED " + query);
      }

      public void onEvent(Event e, Closeable closeable)
      {
        JSONObject json = new JSONObject(e.data().data().value().decodeString("utf8"));
        final String id = json.getString("id");
        IndexResponse resp = prepareIndex(buildElasticSearchIndexName(indexName, indexBy, json), typeName, id, json);
        system.log().info(new Boolean(resp.status().toString() == "OK").toString());
        system.log().info("finished " + e.data().eventId());
        setEventStoreCheckpoint(indexName, indexBy, typeName, e.number().value());
      }

      public void onError(Throwable err)
      {
        throw new RuntimeException(err.toString());
      }

      public void onClose()
      {
        system.log().info("ElasticSearch  connection closed");
      }
    }, getEventStoreCheckpoint(indexName, indexBy, typeName), true, null);
  }

  public void close() {
    client.close();
  }
}
