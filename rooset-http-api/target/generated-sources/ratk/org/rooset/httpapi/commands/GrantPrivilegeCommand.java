
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class GrantPrivilegeCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final java.util.UUID memberId;
  public final java.lang.Boolean pollingRight;
  public final java.lang.Boolean votingRight;
  public final java.lang.Boolean initiativeRight;
  public final java.lang.Boolean managementRight;
  public final java.lang.Long weight;


  @JsonCreator
  
    public GrantPrivilegeCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("memberId") java.util.UUID memberId,
          @JsonProperty("pollingRight") java.lang.Boolean pollingRight,
          @JsonProperty("votingRight") java.lang.Boolean votingRight,
          @JsonProperty("initiativeRight") java.lang.Boolean initiativeRight,
          @JsonProperty("managementRight") java.lang.Boolean managementRight,
          @JsonProperty("weight") java.lang.Long weight)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.memberId = memberId;
      this.pollingRight = pollingRight;
      this.votingRight = votingRight;
      this.initiativeRight = initiativeRight;
      this.managementRight = managementRight;
      this.weight = weight;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "GRANT_PRIVILEGE_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

