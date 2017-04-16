package org.rooset.eventstoresearchbridge;

import com.sun.imageio.stream.StreamCloser;

import java.io.Closeable;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.concurrent.TimeUnit;

import akka.actor.ActorSystem;
import akka.stream.impl.io.ByteStringParser;
import akka.util.ByteString;
import eventstore.Content;
import eventstore.Event;
import eventstore.IndexedEvent;
import eventstore.SubscriptionObserver;
import eventstore.j.EsConnection;
import eventstore.j.EsConnectionFactory;
import eventstore.util.ByteStringToString;

import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.transport.client.PreBuiltTransportClient;
import org.json.JSONObject;

/**
 * Hello world!
 *
 */
public class App 
{
  public static void main( String[] args )
  {
    System.out.println("Hello World!");

    final ActorSystem system = ActorSystem.create();
    final EsConnection connection = EsConnectionFactory.create(system);
    final ElasticSearchService elasticSearchService = new ElasticSearchService();

    final Closeable closeable = connection.subscribeToStreamFrom("$projections-ISSUE_QUERY-result", new SubscriptionObserver<Event>()
    {
      public void onLiveProcessingStart(Closeable closeable)
      {
        system.log().info("live processing started");
      }

      public void onEvent(Event e, Closeable closeable)
      {
        system.log().info("Event happened");
        system.log().info(e.data().data().value().decodeString("utf8"));
        JSONObject json = new JSONObject(e.data().data().value().decodeString("utf8"));
        final String id = json.getString("id");
        elasticSearchService.prepareIndex("issue_query", "state", id, json);
        system.log().info("processing " + e.data().eventId());
        system.log().info("finished " + e.data().eventId());
      }

      public void onError(Throwable err)
      {
        system.log().error(err.toString());

      }

      public void onClose()
      {
        system.log().info("onClose");
      }
    },0, true, null);
  }
}
