package org.rooset.httpapi.models;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

import java.util.List;
import java.util.UUID;

public class SchulzeBallot
{
  public final List<List<UUID>> approve;
  public final List<UUID> abstain;
  public final List<List<UUID>> disapprove;

  @JsonCreator
  public SchulzeBallot(
      @JsonProperty("approve") List<List<UUID>> approve,
      @JsonProperty("abstain") List<UUID> abstain,
      @JsonProperty("disapprove") List<List<UUID>> disapprove)
  {
    this.approve = approve;
    this.abstain = abstain;
    this.disapprove = disapprove;
  }
}
