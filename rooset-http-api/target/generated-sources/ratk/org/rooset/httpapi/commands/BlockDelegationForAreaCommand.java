
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class BlockDelegationForAreaCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID areaId;
  public final java.util.UUID requesterId;


  @JsonCreator
  
    public BlockDelegationForAreaCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("areaId") java.util.UUID areaId,
          @JsonProperty("requesterId") java.util.UUID requesterId)
    {
      this.id = id;
      this.areaId = areaId;
      this.requesterId = requesterId;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "BLOCK_DELEGATION_FOR_AREA_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

