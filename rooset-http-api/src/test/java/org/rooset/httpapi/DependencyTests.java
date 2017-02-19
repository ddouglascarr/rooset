package org.rooset.httpapi;

import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.core.JsonProcessingException;

import org.json.JSONObject;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.enums.ExceptionType;
import org.rooset.httpapi.exceptions.CommandEvaluationException;
import org.rooset.httpapi.models.SchulzeBallot;
import org.rooset.httpapi.models.UserModel;
import org.skyscreamer.jsonassert.JSONAssert;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import static org.junit.Assert.*;

@RunWith(SpringRunner.class)
@ActiveProfiles("integration-tests")
@SpringBootTest
public class DependencyTests
{


  @Test
  public void contextLoads()
  {}



  @Test
  public void JacksonObjectMapperTest() throws Exception
  {
    ObjectMapper mapper = new ObjectMapper();
    UserModel user = new UserModel(
        "no1TestUser420", "foo", "bar", "foo@bar.com", "Should be encoded");
    final UUID id = UUID.randomUUID();
    user.setId(id);
    String json = mapper.writeValueAsString(user);
    UserModel parsedUser = mapper.readValue(json, UserModel.class);
    assertEquals(parsedUser.getId(), user.getId());
    assertEquals(parsedUser.getEmail(), user.getEmail());
    assertEquals(parsedUser.getLastName(), "bar");
    assertEquals(parsedUser.getId(), id);
  }



  @Test
  public void JSONObjectMapperTest() throws Exception
  {
    UserModel user = new UserModel(
        "no1TestUser420", "foo", "bar", "foo@bar.com", "Should be encoded");
    user.setId(UUID.fromString("464b1ebb-32c1-460c-8e9e-444444444444"));

    ObjectMapper mapper = new ObjectMapper();
    mapper.registerModule(new JsonOrgModule());
    JSONObject json = mapper.convertValue(user, JSONObject.class);
    assertEquals("foo", json.get("firstName"));
    assertEquals("foo@bar.com", json.get("email"));

    assertEquals(new JSONObject()
        .put("username", "no1TestUser420")
        .put("firstName", "foo")
        .put("lastName", "bar")
        .put("email", "foo@bar.com")
        .put("password", "Should be encoded")
        .put("id", "464b1ebb-32c1-460c-8e9e-444444444444")
        .toString(),
        json.toString());
  }



  @Test
  public void JSONObjectMapperVisibilityTest() throws Exception
  {
    class TestObject
    {
      private ObjectMapper objectMapper = new ObjectMapper();
      public final String msg;
      public TestObject(String msg)
      {
        this.msg = msg;
        objectMapper
            .registerModule(new JsonOrgModule())
            .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
      }

      public JSONObject serialize()
      {
        return objectMapper.convertValue(this, JSONObject.class);
      }

    }

    final JSONObject expected = new JSONObject()
        .put("msg", "foobar");
    TestObject testObject = new TestObject("foobar");

    assertEquals(expected.toString(), testObject.serialize().toString());
  }



  @Test
  public void JSONObjectSerializeSchulzeBallot() throws Exception
  {
    ObjectMapper objectMapper = new ObjectMapper();
    objectMapper
        .registerModule(new JsonOrgModule())
        .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);

    List<List<UUID>> approve = new ArrayList<>();
    List<UUID> abstain = new ArrayList<>();
    List<List<UUID>> disapprove = new ArrayList<>();
    List<UUID> approve0 = new ArrayList();
    approve0.add(UUID.fromString("464b1ebb-32c1-460c-8e9e-000000000000"));
    approve.add(approve0);
    SchulzeBallot ballot = new SchulzeBallot(approve, abstain, disapprove);

    JSONObject result = objectMapper.convertValue(ballot, JSONObject.class);
    assertEquals("[[\"464b1ebb-32c1-460c-8e9e-000000000000\"]]", result.get("approve").toString());
  }



  @Test
  public void JSONObjectMapperNestedClasses() throws Exception
  {

    ObjectMapper objectMapper = new ObjectMapper();
    objectMapper
        .registerModule(new JsonOrgModule())
        .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    final String strJson = "{\"msg\":\"foobarMessage\",\"schulzeBallot\":{\"approve\":[[\"464b1ebb-32c1-460c-8e9e-000000000000\"],[\"464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa\"]],\"abstain\":[\"464b1ebb-32c1-460c-8e9e-444444444444\"],\"disapprove\":[[\"464b1ebb-32c1-460c-8e9e-888888888888\"]]}}";
    final JSONObject expectedJson = new JSONObject(strJson);

    TestObject parsedTestObject = objectMapper.readValue(strJson, TestObject.class);
    assertEquals("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa", parsedTestObject.schulzeBallot.approve.get(1).get(0).toString());
    assertEquals("464b1ebb-32c1-460c-8e9e-444444444444", parsedTestObject.schulzeBallot.abstain.get(0).toString());

    JSONObject resultJson = objectMapper.convertValue(parsedTestObject, JSONObject.class);

    assertEquals(strJson, resultJson.toString());
  }



  @Test
  public void ExceptionSerializeTest() throws Exception
  {
    CommandEvaluationException exc = new CommandEvaluationException(
        ExceptionCode.ITEM_NOT_FOUND_EXCEPTION, "foo bar");
    JSONObject json = exc.serialize();
    JSONObject expectedJson = new JSONObject();
    expectedJson
        .put("error", true)
        .put("type", ExceptionType.COMMAND_EVALUATION_EXCEPTION)
        .put("payload", new JSONObject()
            .put("exceptionCode", ExceptionCode.ITEM_NOT_FOUND_EXCEPTION)
            .put("message", "foo bar"));
    assertEquals(expectedJson.get("type").toString(), json.get("type").toString());
  }



  @Test
  public void JSONAssertShouldPassRegardlessOfOrder() throws Exception
  {
    String a = "{\"a\":1,\"b\":2}";
    String b = "{\"b\":2,\"a\":1}";
    JSONAssert.assertEquals(a, new JSONObject(a), true);
    JSONAssert.assertEquals(b, new JSONObject(a), true);

    JSONObject c = new JSONObject()
        .put("a", 1)
        .put("b", 3);
    JSONAssert.assertNotEquals(a, c, true);
  }



}

class TestObject
{
  public final String msg;
  public final SchulzeBallot schulzeBallot;

  @JsonCreator
  public TestObject(
      @JsonProperty("msg") String msg,
      @JsonProperty("schulzeBallot") SchulzeBallot schulzeBallot)
  {
    this.msg = msg;
    this.schulzeBallot = schulzeBallot;
  }
}
