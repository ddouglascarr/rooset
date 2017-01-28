package org.rooset.httpapi;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.rooset.httpapi.models.UserModel;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.io.IOException;
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
}
