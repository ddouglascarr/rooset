
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class SetIssueBallotCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final org.rooset.httpapi.models.SchulzeBallot ballot;


  @JsonCreator
  
    public SetIssueBallotCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("ballot") org.rooset.httpapi.models.SchulzeBallot ballot)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.ballot = ballot;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "SET_ISSUE_BALLOT_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

