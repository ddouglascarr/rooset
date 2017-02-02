
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CreateNewInitiativeCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final java.util.UUID initiativeId;
  public final java.util.UUID unitId;
  public final java.util.UUID areaId;
  public final java.util.UUID policyId;
  public final String name;
  public final java.lang.Boolean polling;
  public final String externalReference;
  public final String content;
  public final String textSearchData;
  public final java.lang.Long created;


  @JsonCreator
  
    public CreateNewInitiativeCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("initiativeId") java.util.UUID initiativeId,
          @JsonProperty("unitId") java.util.UUID unitId,
          @JsonProperty("areaId") java.util.UUID areaId,
          @JsonProperty("policyId") java.util.UUID policyId,
          @JsonProperty("name") String name,
          @JsonProperty("polling") java.lang.Boolean polling,
          @JsonProperty("externalReference") String externalReference,
          @JsonProperty("content") String content,
          @JsonProperty("textSearchData") String textSearchData,
          @JsonProperty("created") java.lang.Long created)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.initiativeId = initiativeId;
      this.unitId = unitId;
      this.areaId = areaId;
      this.policyId = policyId;
      this.name = name;
      this.polling = polling;
      this.externalReference = externalReference;
      this.content = content;
      this.textSearchData = textSearchData;
      this.created = created;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "CREATE_NEW_INITIATIVE_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

