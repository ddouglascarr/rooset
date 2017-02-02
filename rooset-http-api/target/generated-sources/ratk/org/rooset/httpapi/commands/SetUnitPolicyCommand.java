
package org.rooset.httpapi.commands;

import org.json.JSONObject;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsonorg.JsonOrgModule;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class SetUnitPolicyCommand
{
  private ObjectMapper objectMapper = new ObjectMapper();

  public final java.util.UUID id;
  public final java.util.UUID requesterId;
  public final java.util.UUID policyId;
  public final String name;
  public final String description;
  public final java.lang.Boolean polling;
  public final java.lang.Long maxAdmissionTime;
  public final java.lang.Long minAdmissionTime;
  public final java.lang.Long discussionTime;
  public final java.lang.Long verificationTime;
  public final java.lang.Long votingTime;
  public final java.lang.Long issueQuorumNum;
  public final java.lang.Long issueQuorumDen;
  public final org.rooset.httpapi.enums.DefeatStrength defeatStrength;
  public final java.lang.Long directMajorityNum;
  public final java.lang.Long directMajorityDen;
  public final java.lang.Boolean directMajorityStrict;
  public final java.lang.Long directMajorityPositive;
  public final java.lang.Long directMajorityNonNegative;
  public final java.lang.Boolean noReverseBeatPath;
  public final java.lang.Boolean noMultistageMajority;


  @JsonCreator
  
    public SetUnitPolicyCommand(
        @JsonProperty("id") java.util.UUID id,
          @JsonProperty("requesterId") java.util.UUID requesterId,
          @JsonProperty("policyId") java.util.UUID policyId,
          @JsonProperty("name") String name,
          @JsonProperty("description") String description,
          @JsonProperty("polling") java.lang.Boolean polling,
          @JsonProperty("maxAdmissionTime") java.lang.Long maxAdmissionTime,
          @JsonProperty("minAdmissionTime") java.lang.Long minAdmissionTime,
          @JsonProperty("discussionTime") java.lang.Long discussionTime,
          @JsonProperty("verificationTime") java.lang.Long verificationTime,
          @JsonProperty("votingTime") java.lang.Long votingTime,
          @JsonProperty("issueQuorumNum") java.lang.Long issueQuorumNum,
          @JsonProperty("issueQuorumDen") java.lang.Long issueQuorumDen,
          @JsonProperty("defeatStrength") org.rooset.httpapi.enums.DefeatStrength defeatStrength,
          @JsonProperty("directMajorityNum") java.lang.Long directMajorityNum,
          @JsonProperty("directMajorityDen") java.lang.Long directMajorityDen,
          @JsonProperty("directMajorityStrict") java.lang.Boolean directMajorityStrict,
          @JsonProperty("directMajorityPositive") java.lang.Long directMajorityPositive,
          @JsonProperty("directMajorityNonNegative") java.lang.Long directMajorityNonNegative,
          @JsonProperty("noReverseBeatPath") java.lang.Boolean noReverseBeatPath,
          @JsonProperty("noMultistageMajority") java.lang.Boolean noMultistageMajority)
    {
      this.id = id;
      this.requesterId = requesterId;
      this.policyId = policyId;
      this.name = name;
      this.description = description;
      this.polling = polling;
      this.maxAdmissionTime = maxAdmissionTime;
      this.minAdmissionTime = minAdmissionTime;
      this.discussionTime = discussionTime;
      this.verificationTime = verificationTime;
      this.votingTime = votingTime;
      this.issueQuorumNum = issueQuorumNum;
      this.issueQuorumDen = issueQuorumDen;
      this.defeatStrength = defeatStrength;
      this.directMajorityNum = directMajorityNum;
      this.directMajorityDen = directMajorityDen;
      this.directMajorityStrict = directMajorityStrict;
      this.directMajorityPositive = directMajorityPositive;
      this.directMajorityNonNegative = directMajorityNonNegative;
      this.noReverseBeatPath = noReverseBeatPath;
      this.noMultistageMajority = noMultistageMajority;

      objectMapper
          .registerModule(new JsonOrgModule())
          .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.DEFAULT);
    }
  

  public JSONObject serialize()
  {
    JSONObject out = new JSONObject()
        .put("type", "SET_UNIT_POLICY_COMMAND");

    JSONObject payload = objectMapper.convertValue(this, JSONObject.class);
    out.put("payload", payload);
    return out;
  }
}

