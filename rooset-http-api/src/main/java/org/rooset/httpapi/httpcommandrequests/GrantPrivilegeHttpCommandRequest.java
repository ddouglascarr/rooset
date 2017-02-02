package org.rooset.httpapi.httpcommandrequests;

import java.util.UUID;

public class GrantPrivilegeHttpCommandRequest
{
  private UUID memberId;
  private Boolean pollingRight;
  private Boolean votingRight;
  private Boolean initiativeRight;
  private Boolean managementRight;
  private Long weight;

  public GrantPrivilegeHttpCommandRequest()
  {
  }

  public UUID getMemberId()
  {
    return memberId;
  }

  public void setMemberId(UUID memberId)
  {
    this.memberId = memberId;
  }

  public Boolean getPollingRight()
  {
    return pollingRight;
  }

  public void setPollingRight(Boolean pollingRight)
  {
    this.pollingRight = pollingRight;
  }

  public Boolean getVotingRight()
  {
    return votingRight;
  }

  public void setVotingRight(Boolean votingRight)
  {
    this.votingRight = votingRight;
  }

  public Boolean getInitiativeRight()
  {
    return initiativeRight;
  }

  public void setInitiativeRight(Boolean initiativeRight)
  {
    this.initiativeRight = initiativeRight;
  }

  public Boolean getManagementRight()
  {
    return managementRight;
  }

  public void setManagementRight(Boolean managementRight)
  {
    this.managementRight = managementRight;
  }

  public Long getWeight()
  {
    return weight;
  }

  public void setWeight(Long weight)
  {
    this.weight = weight;
  }
}
