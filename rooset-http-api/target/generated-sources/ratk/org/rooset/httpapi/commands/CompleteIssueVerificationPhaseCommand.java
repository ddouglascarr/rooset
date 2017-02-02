
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CompleteIssueVerificationPhaseCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;


  @JsonCreator
  
    public CompleteIssueVerificationPhaseCommand(
        @JsonProperty("id") java.util.UUID id)
    {
      this.id = id;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "COMPLETE_ISSUE_VERIFICATION_PHASE_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

