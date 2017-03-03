package org.rooset.httpapi.controllers;

import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.skyscreamer.jsonassert.JSONAssert;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.UUID;
import static org.junit.Assert.*;

@RunWith(SpringRunner.class)
@ActiveProfiles("integration-tests")
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class UserControllerTests
{

  @Autowired
  private TestRestTemplate restTemplate;

  @Autowired
  private UserRepository userRepository;

  @Autowired
  private PasswordEncoder passwordEncoder;

  private UUID userId = UUID.randomUUID();

  @Before
  public void start() throws Exception
  {
    UserModel testUser = new UserModel(
        "testUser", "Test User",
        "Test", "User", "test@example.com",
        passwordEncoder.encode("password1"));
    testUser.setId(userId);
    userRepository.save(testUser);
  }



  @After
  public void end() throws Exception
  {
    userRepository.delete(userId);
  }

  @Test
  public void getUserTest() throws Exception
  {
    JSONObject expectedHttpRespBody = new JSONObject();
    expectedHttpRespBody.put("id", userId.toString());
    expectedHttpRespBody.put("username", "testUser");
    expectedHttpRespBody.put("displayName", "Test User");

    ResponseEntity<String> response = this.restTemplate.getForEntity(
        "/users/" + userId.toString(), String.class);
    JSONObject responseBody = new JSONObject(response.getBody());

    assertTrue("2xx expected, got " + response.getStatusCode().toString(),
        response.getStatusCode().is2xxSuccessful());
    JSONAssert.assertEquals(expectedHttpRespBody, responseBody, true);
  }



  @Test
  public void getSessionShould401WhenUnauthenticated() throws Exception
  {
    ResponseEntity<String> response =  this.restTemplate.getForEntity(
        "/session", String.class);

    assertEquals("should be 401, was: " + response.getStatusCode().toString(),
        response.getStatusCode(), HttpStatus.UNAUTHORIZED);
  }



  @Test
  public void getSessionTest()
  {

    JSONObject expectedRespBody = new JSONObject();
    expectedRespBody.put("id", userId.toString());
    expectedRespBody.put("username", "testUser");
    expectedRespBody.put("displayName", "Test User");

    ResponseEntity<String> response = this.restTemplate
        .withBasicAuth("testUser", "password1")
        .getForEntity("/session", String.class);
    JSONObject responseBody = new JSONObject(response.getBody());

    assertTrue("2xx expected, got " + response.getStatusCode().toString(),
        response.getStatusCode().is2xxSuccessful());
    JSONAssert.assertEquals(expectedRespBody, responseBody, true);
  }


}
