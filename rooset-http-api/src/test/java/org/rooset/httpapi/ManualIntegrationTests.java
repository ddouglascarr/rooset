package org.rooset.httpapi;

import org.apache.commons.exec.ExecuteWatchdog;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.repositories.UserRepository;
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
import org.springframework.security.core.userdetails.UserDetailsService;
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

  @Autowired
  private UserRepository userRepository;

  private ExecuteWatchdog watchdog;

  @Before
  public void startGetEventStore() throws Exception
  {
    System.out.println("Starting Event Store");
    watchdog = testingEventStoreService.startTestingEventStore();
  }

  @After
  public void killGetEventStore() throws Exception
  {
    System.out.println("Killing event store");
    watchdog.destroyProcess();
  }

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



  @Test
  public void grantPermissionShouldWork() throws Exception
  {
    UUID requesterId = userRepository.findOneByEmail("foo@bar.com").getId();
    JSONObject reqBody = new JSONObject()
        .put("memberId", "464b1ebb-32c1-460c-8e9e-222222222222")
        .put("pollingRight", true)
        .put("votingRight", true)
        .put("initiativeRight", true)
        .put("managementRight", false)
        .put("weight", 1);

    JSONObject unitCreatedEvt = new JSONObject();
    unitCreatedEvt.put("type", "UNIT_CREATED_EVENT");
    unitCreatedEvt.put("payload", new JSONObject()
        .put("id", "464b1ebb-32c1-460c-8e9e-111111111111")
        .put("requesterId", requesterId.toString())
        .put("name", "Test Unit")
        .put("description", "The Test Unit"));
    testingEventStoreService.saveEvent(unitCreatedEvt);

    HttpHeaders headers = new HttpHeaders();
    headers.add("Content-Type", "application/json");
    headers.add("Accept", "application/json");
    HttpEntity<String> req = new HttpEntity<>(reqBody.toString(), headers);
    ResponseEntity<String> resp = this.restTemplate.postForEntity(
        "/units/464b1ebb-32c1-460c-8e9e-111111111111/members", req, String.class);
    assertTrue(resp.getStatusCode().is2xxSuccessful());

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
