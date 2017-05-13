package org.rooset.httpapi.services;

import org.rooset.httpapi.exceptions.RatkException;

public interface SearchService {

  String performSearch(
      final String indexName,
      final String indexByValue,
      final String typeName,
      final String queryBody)
      throws RatkException;

}
