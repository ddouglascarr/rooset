package org.rooset.httpapi.httpcommandrequests;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

import java.util.UUID;

public class GrantPrivilegeHttpCommandRequest
{
  public final UUID memberId;
  public final Boolean pollingRight;
  public final Boolean votingRight;
  public final Boolean initiativeRight;
  public final Boolean managementRight;
  public final Long weight;

  @JsonCreator
  public GrantPrivilegeHttpCommandRequest(
      @JsonProperty("memberId") UUID memberId,
      @JsonProperty("pollingRight") Boolean pollingRight,
      @JsonProperty("votingRight") Boolean votingRight,
      @JsonProperty("initiativeRight") Boolean initiativeRight,
      @JsonProperty("managementRight") Boolean managementRight,
      @JsonProperty("weight") Long weight)
  {
    this.memberId = memberId;
    this.pollingRight = pollingRight;
    this.votingRight = votingRight;
    this.initiativeRight = initiativeRight;
    this.managementRight = managementRight;
    this.weight = weight;
  }
}
