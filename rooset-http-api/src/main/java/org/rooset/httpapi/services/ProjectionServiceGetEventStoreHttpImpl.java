package org.rooset.httpapi.services;


import org.apache.http.StatusLine;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
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
public class ProjectionServiceGetEventStoreHttpImpl implements ProjectionService
{

  @Value("${get-event-store.host}")
  private String getEventStoreHost;

  @Value("${get-event-store.port}")
  private String getEventStorePort;



  @Override
  public String query(String uri) throws RatkException
  {
    try {
      CloseableHttpClient client = HttpClients.custom().build();
      try {
        HttpGet get = new HttpGet("http://" + getEventStoreHost + ":" + getEventStorePort + uri);
        get.addHeader("Accept", "application/json");
        CloseableHttpResponse resp = client.execute(get);
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
