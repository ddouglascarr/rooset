
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CompleteIssueAdmissionPhaseCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;


  @JsonCreator
  
    public CompleteIssueAdmissionPhaseCommand(
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
        .put("type", "COMPLETE_ISSUE_ADMISSION_PHASE_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

