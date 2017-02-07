const { camelCase } = require('lodash');
const generateTestMethod = require('./generateTestMethod');

module.exports = (baseJavaPkg, testDoc) =>  {
  const testClassname = camelCase(testDoc.label);
  const testMethods = testDoc.scenarios
      .map((scenario) => generateTestMethod(scenario))
      .join('\n\n\n');


  return `
package ${baseJavaPkg}.integrationtests;

import org.apache.commons.exec.ExecuteWatchdog;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.rooset.httpapi.services.TestingEventStoreService;
import org.rooset.httpapi.services.IdService;
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
import org.springframework.http.HttpMethod;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;
import org.skyscreamer.jsonassert.JSONAssert;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import org.mockito.Mockito;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;


@RunWith(SpringRunner.class)
@ActiveProfiles("integration-tests")
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class ${testClassname}
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

  private HttpHeaders headers;


  @Before
  public void startGetEventStore() throws Exception
  {
    System.out.println("Starting Event Store");
    watchdog = testingEventStoreService.startTestingEventStore();

    testUser = new UserModel(
      "no1TestUser420", "foo", "bar", "foo@bar.com",
      passwordEncoder.encode("password1"));

    headers = new HttpHeaders();
    headers.add("Content-Type", "application/json");
    headers.add("Accept", "application/json");
  }



  @After
  public void killGetEventStore() throws Exception
  {
    System.out.println("Killing event store");
    watchdog.destroyProcess();

    userRepository.delete(testUser);
  }




${testMethods}




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

`;

}
