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

import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.rest.RestStatus;
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
    elasticSearchService.subscribeToQueryStream("ISSUE_QUERY");

  }
}
