package org.rooset.httpapi.controllers;

import org.json.JSONObject;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.skyscreamer.jsonassert.JSONAssert;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.ResponseEntity;
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



  @Test
  public void getUserTest()
  {
    UUID userId = UUID.randomUUID();
    UserModel testUser = new UserModel(
        "testUser", "Test User",
        "Test", "User", "test@example.com",
        "encryptedPassword");
    testUser.setId(userId);
    userRepository.save(testUser);

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



}
