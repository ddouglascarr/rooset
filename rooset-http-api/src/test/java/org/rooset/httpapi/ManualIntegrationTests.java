package org.rooset.httpapi;

import org.json.JSONObject;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.services.TestingEventStoreService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.boot.web.client.RestTemplateBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@RunWith(SpringRunner.class)
@ActiveProfiles("integration-tests")
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class ManualIntegrationTests
{

  @Autowired
  private TestRestTemplate restTemplate;

  @Autowired
  private TestingEventStoreService testingEventStoreService;



  @Test
  public void createUnitShouldFailOnInvalidBody() throws Exception
  {
    Map<String, String> reqBody = new HashMap<>();
    reqBody.put("foo", "bar");
    ResponseEntity<String> resp = this.restTemplate.postForEntity("/units", reqBody, String.class);
    String body = resp.getBody();

    assertEquals(HttpStatus.BAD_REQUEST, resp.getStatusCode());
  }



  @Test
  public void createUnitShouldCreateUnit() throws Exception
  {
    JSONObject reqBody = new JSONObject();
    reqBody.put("name", "Test");
    reqBody.put("description", "The Test");
    HttpHeaders headers = new HttpHeaders();
    headers.add("Content-Type", "application/json");
    headers.add("Accept", "application/json");
    HttpEntity<String> req = new HttpEntity<>(reqBody.toString(), headers);
    ResponseEntity<String> resp = this.restTemplate.postForEntity("/units", req, String.class);

    assertTrue(resp.getStatusCode().is2xxSuccessful());

    JSONObject respBody = new JSONObject(resp.getBody());
    UUID id = UUID.fromString(respBody.getString("id"));
    JSONObject event = testingEventStoreService.getLastEventForAggregate(id);
    assertEquals(event.getJSONObject("payload").getString("id"), id.toString());
    assertEquals(event.getJSONObject("payload").getString("name"), "Test");
  }



  @TestConfiguration
  static class Config
  {
    @Bean
    public RestTemplateBuilder restTemplateBuilder()
    {
      return new RestTemplateBuilder()
          .basicAuthorization("foo@bar.com", "password1");
    }
  }



}
