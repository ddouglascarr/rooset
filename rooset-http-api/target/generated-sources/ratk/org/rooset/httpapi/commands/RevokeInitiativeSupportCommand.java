
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class RevokeInitiativeSupportCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final java.util.UUID initiativeId;


  @JsonCreator
  
    public RevokeInitiativeSupportCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("initiativeId") java.util.UUID initiativeId)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.initiativeId = initiativeId;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "REVOKE_INITIATIVE_SUPPORT_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

