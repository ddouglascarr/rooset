package org.rooset.httpapi;

import com.fasterxml.jackson.annotation.JsonAutoDetect;
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
import org.rooset.httpapi.models.UserModel;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.UUID;

import static org.junit.Assert.*;

@RunWith(SpringRunner.class)
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
        "foo", "bar", "foo@bar.com", "Should be encoded");
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
        "foo", "bar", "foo@bar.com", "Should be encoded");

    ObjectMapper mapper = new ObjectMapper();
    mapper.registerModule(new JsonOrgModule());
    JSONObject json = mapper.convertValue(user, JSONObject.class);
    assertEquals("foo", json.get("firstName"));
    assertEquals("foo@bar.com", json.get("email"));

    assertEquals(new JSONObject()
        .put("firstName", "foo")
        .put("lastName", "bar")
        .put("email", "foo@bar.com")
        .put("password", "Should be encoded")
        .put("id", JSONObject.NULL)
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
  public void ExceptionToStringTest() throws Exception
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
}
