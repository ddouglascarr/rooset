package org.rooset.eventstoresearchbridge;


/**
 * Hello world!
 *
 */
public class App 
{
  public static void main( String[] args )
  {
    if(args.length != 1) {
      throw new RuntimeException("Must have 1 arguemnt, query name (eg ISSUE_QUERY)");
    }
    String queryName = args[0];
    final ElasticSearchService elasticSearchService = new ElasticSearchService();
    elasticSearchService.subscribeToQueryStream(queryName);
  }
}
