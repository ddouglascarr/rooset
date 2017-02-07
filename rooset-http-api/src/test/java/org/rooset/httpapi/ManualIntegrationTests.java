package org.rooset.httpapi;

import org.apache.commons.exec.ExecuteWatchdog;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.rooset.httpapi.models.UserDetailsImpl;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.rooset.httpapi.services.IdService;
import org.rooset.httpapi.services.TestingEventStoreService;
import org.rooset.httpapi.services.UserDetailsServiceImpl;
import org.rooset.httpapi.commandcontrollers.CreateUnitHttpCommandRequestController;
import org.skyscreamer.jsonassert.JSONAssert;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.context.TestConfiguration;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.boot.web.client.RestTemplateBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Primary;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
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

  @Autowired
  private PasswordEncoder passwordEncoder;

  @Autowired
  private IdService idService;

  private ExecuteWatchdog watchdog;
  private UserModel testUser;

  @Before
  public void startGetEventStore() throws Exception
  {
    System.out.println("Starting Event Store");
    watchdog = testingEventStoreService.startTestingEventStore();
    // MockitoAnnotations.initMocks(this);

    testUser = new UserModel(
      "no1TestUser420", "foo", "bar", "foo@bar.com",
      passwordEncoder.encode("password1"));
  }

  @After
  public void killGetEventStore() throws Exception
  {
    System.out.println("Killing event store");
    watchdog.destroyProcess();

    userRepository.delete(testUser);
  }

  @Test
  public void createUnitShouldFailOnInvalidBody() throws Exception
  {
    userRepository.save(testUser);
    Map<String, String> reqBody = new HashMap<>();
    reqBody.put("foo", "bar");
    ResponseEntity<String> resp = this.restTemplate.postForEntity("/units", reqBody, String.class);
    String body = resp.getBody();

    assertEquals(HttpStatus.BAD_REQUEST, resp.getStatusCode());
  }



  @Test
  public void createUnitShouldCreateUnit() throws Exception
  {
    testUser.setId(UUID.fromString("464b1ebb-32c1-460c-8e9e-555555555555"));
    userRepository.save(testUser);
    UUID id = UUID.fromString("464b1ebb-32c1-460c-8e9e-333333333333");
    when(idService.generateUniqueId()).thenReturn(id);
    JSONObject reqBody = new JSONObject();
    reqBody.put("name", "Test");
    reqBody.put("description", "The Test");
    HttpHeaders headers = new HttpHeaders();
    headers.add("Content-Type", "application/json");
    headers.add("Accept", "application/json");
    HttpEntity<String> req = new HttpEntity<>(reqBody.toString(), headers);
    ResponseEntity<String> resp = this.restTemplate.postForEntity("/units", req, String.class);

    JSONObject expectedHttpRespBody = new JSONObject();
    expectedHttpRespBody.put("id", id.toString());
    expectedHttpRespBody.put("name", "Test");
    expectedHttpRespBody.put("description", "The Test");
    expectedHttpRespBody.put("requesterId", "464b1ebb-32c1-460c-8e9e-555555555555");

    assertTrue(resp.getStatusCode().is2xxSuccessful());

    JSONObject respBody = new JSONObject(resp.getBody());
    JSONAssert.assertEquals(expectedHttpRespBody, respBody, true);
    JSONObject event = testingEventStoreService.getLastEventForAggregate(id);
    assertEquals(event.getJSONObject("payload").getString("id"), "464b1ebb-32c1-460c-8e9e-333333333333");
    assertEquals(event.getJSONObject("payload").getString("name"), "Test");
  }



  @Test
  public void grantPermissionShouldWork() throws Exception
  {
    testUser.setId(UUID.fromString("464b1ebb-32c1-460c-8e9e-333333333333"));
    userRepository.save(testUser);
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
        .put("requesterId", UUID.fromString("464b1ebb-32c1-460c-8e9e-333333333333"))
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
          .basicAuthorization("no1TestUser420", "password1");
    }

    @Bean
    @Primary
    public IdService idService()
    {
      return Mockito.mock(IdService.class);
    }
  }



}
