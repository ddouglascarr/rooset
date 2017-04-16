package org.rooset.eventstoresearchbridge;

import com.sun.imageio.stream.StreamCloser;

import java.io.Closeable;

import akka.actor.ActorSystem;
import eventstore.Event;
import eventstore.IndexedEvent;
import eventstore.SubscriptionObserver;
import eventstore.j.EsConnection;
import eventstore.j.EsConnectionFactory;

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
    final Closeable closeable = connection.subscribeToStream("$projections-ISSUE_QUERY-result", new SubscriptionObserver<Event>()
    {
      public void onLiveProcessingStart(Closeable closeable)
      {
        system.log().info("live processing started");
      }

      public void onEvent(Event e, Closeable closeable)
      {
        system.log().info("Event happened");
        system.log().info(e.data().toString());
      }

      public void onError(Throwable err)
      {
        system.log().error(err.toString());

      }

      public void onClose()
      {
        system.log().info("onClose");
      }
    }, false, null);
  }
}
