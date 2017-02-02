
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class UnsetIssueDelegationCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;


  @JsonCreator
  
    public UnsetIssueDelegationCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId)
    {
      this.id = id;
      this.requesterId = requesterId;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "UNSET_ISSUE_DELEGATION_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

