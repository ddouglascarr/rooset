
package org.rooset.httpapi.httpcommandrequestbodies;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class CreateUnitHttpCommandRequestBody
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final String name;
  public final String description;

  @JsonCreator
  
    public CreateUnitHttpCommandRequestBody(
        @JsonProperty("name") String name,
          @JsonProperty("description") String description)
    {
      this.name = name;
      this.description = description;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

}

