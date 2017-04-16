package org.rooset.eventstoresearchbridge;

import java.io.Closeable;

import akka.actor.ActorSystem;
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
    final Closeable closeable = connection.subscribeToAll(new SubscriptionObserver<IndexedEvent>()
    {
      public void onLiveProcessingStart(Closeable closeable)
      {
        system.log().info("Live processing started");
      }

      public void onEvent(IndexedEvent indexedEvent, Closeable closeable)
      {
        system.log().info(indexedEvent.toString());
      }

      public void onError(Throwable throwable)
      {
        system.log().error(throwable.toString());
      }

      public void onClose()
      {
        system.log().error("Subscription closed");
      }
    }, false, null);

  }
}
