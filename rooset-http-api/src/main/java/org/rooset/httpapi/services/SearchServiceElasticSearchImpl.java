package org.rooset.httpapi.services;

import org.apache.http.HttpEntity;
import org.apache.http.StatusLine;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.exceptions.QueryException;
import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.exceptions.SystemException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;

import java.io.IOException;

@Service
public class SearchServiceElasticSearchImpl implements SearchService
{
  @Value("${rooset-search.host}")
  private String searchHost;

  @Value("${rooset-search.http-port}")
  private String searchPort;

  @Override
  public String performSearch(
      String indexName,
      String indexByValue,
      String typeName,
      String queryBody)
      throws RatkException
  {
    final String path = buildPath(indexName, indexByValue, typeName);
    CloseableHttpClient client = HttpClients.custom().build();
    try {
      try {
        HttpPost post = new HttpPost("http://" + searchHost + ":" + searchPort + path);
        post.addHeader("Content-Type", "application/json");
        post.addHeader("Accept", "appliction/json");
        HttpEntity postBody = new StringEntity(queryBody, ContentType.APPLICATION_JSON);
        post.setEntity(postBody);
        CloseableHttpResponse resp = client.execute(post);
        assertCodeIs2xx(resp.getStatusLine());
        return EntityUtils.toString(resp.getEntity());
      } finally {
        client.close();
      }
    } catch (IOException e) {
      throw new SystemException(
          ExceptionCode.GENERAL_PROJECT_EXCEPTION,
          e.getMessage());
    }
  }

  private String buildPath(String indexName, String indexByValue, String typeName)
  {
    if (null == indexByValue) {
      return "/" + indexName + "/" + typeName + "/_search";
    }
    return "/" + indexName + "-" + indexByValue + "/" + typeName + "/_search";
  }

  private void assertCodeIs2xx(StatusLine statusLine) throws QueryException
  {
    HttpStatus status = HttpStatus.valueOf(statusLine.getStatusCode());
    if (status.is2xxSuccessful()) return;
    if (status.equals(HttpStatus.NOT_FOUND)) throw new QueryException(
        ExceptionCode.ITEM_NOT_FOUND_EXCEPTION,
        "Could not find query item");
    throw new QueryException(
        ExceptionCode.GENERAL_PROJECT_EXCEPTION,
        statusLine.getStatusCode() + ": " + statusLine.getReasonPhrase());
  }
}
