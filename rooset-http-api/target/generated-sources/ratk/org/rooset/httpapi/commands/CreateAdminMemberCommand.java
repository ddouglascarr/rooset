
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CreateAdminMemberCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final String login;
  public final String password;
  public final String name;
  public final java.lang.String notifyEmail;


  @JsonCreator
  
    public CreateAdminMemberCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("login") String login,
          @JsonProperty("password") String password,
          @JsonProperty("name") String name,
          @JsonProperty("notifyEmail") java.lang.String notifyEmail)
    {
      this.id = id;
      this.login = login;
      this.password = password;
      this.name = name;
      this.notifyEmail = notifyEmail;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "CREATE_ADMIN_MEMBER_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

