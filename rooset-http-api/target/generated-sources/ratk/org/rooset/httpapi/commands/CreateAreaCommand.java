
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CreateAreaCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID areaId;
  public final java.util.UUID requesterId;
  public final String name;
  public final String description;
  public final String externalReference;


  @JsonCreator
  
    public CreateAreaCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("areaId") java.util.UUID areaId,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("name") String name,
          @JsonProperty("description") String description,
          @JsonProperty("externalReference") String externalReference)
    {
      this.id = id;
      this.areaId = areaId;
      this.requesterId = requesterId;
      this.name = name;
      this.description = description;
      this.externalReference = externalReference;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "CREATE_AREA_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

