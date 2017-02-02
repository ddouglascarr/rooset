
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class UpdateMemberPasswordCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final String oldPassword;
  public final String newPassword;


  @JsonCreator
  
    public UpdateMemberPasswordCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("oldPassword") String oldPassword,
          @JsonProperty("newPassword") String newPassword)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.oldPassword = oldPassword;
      this.newPassword = newPassword;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "UPDATE_MEMBER_PASSWORD_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

