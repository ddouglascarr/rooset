package org.rooset.eventstoresearchbridge;


import akka.actor.ActorSystem;
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
    final ActorSystem system = ActorSystem.create();
    final EsConnection connection = EsConnectionFactory.create(system);
    final ElasticSearchService elasticSearchService = new ElasticSearchService();
    elasticSearchService.subscribeToQueryStream("ISSUE_QUERY");
  }
}
