package org.rooset.eventstoresearchbridge;


import org.apache.commons.cli.*;

/**
 * Hello world!
 *
 */
public class App 
{
  public static void main( String[] args )
  {

    Options options = new Options();
    Option queryOpt = new Option("q", "query", true, "rooset query (eg ISSUE_QUERY)");
    queryOpt.setRequired(true);
    options.addOption(queryOpt);
    Option typeNameOpt = new Option("t", "type-name", true, "search type name (eg issues)");
    options.addOption(typeNameOpt);
    Option indexByOpt = new Option("i", "index-by", true, "payload attribute to index by (eg unitId)");
    indexByOpt.setRequired(false);
    options.addOption(indexByOpt);
    Option searchIdByOpt = new Option(
            "s", "search-id-by", true, "payload attribute to use as search id (eg initiativeId)");
    options.addOption(searchIdByOpt);
    Option indexNameOpt = new Option("n", "index-name", true, "name of index (eg. rooset-unit)");
    indexNameOpt.setRequired(true);
    options.addOption(indexNameOpt);

    CommandLineParser parser = new DefaultParser();
    HelpFormatter formatter = new HelpFormatter();
    CommandLine cmd;

    try {
      cmd = parser.parse(options, args);
    } catch (ParseException e) {
      formatter.printHelp("java -jar <jar_file> [options]", options);
      System.exit(1);
      return;
    }

    String query = cmd.getOptionValue("query");
    String typeName = cmd.getOptionValue("type-name");
    String indexBy = cmd.getOptionValue("index-by");
    String indexName = cmd.getOptionValue("index-name");
    String searchIdBy = cmd.getOptionValue("search-id-by");

    final ElasticSearchService elasticSearchService = new ElasticSearchService();
    elasticSearchService.subscribeToQueryStream(indexName, indexBy, typeName, searchIdBy, query);
  }
}
