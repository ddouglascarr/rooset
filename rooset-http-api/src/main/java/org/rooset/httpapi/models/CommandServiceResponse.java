package org.rooset.httpapi.models;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

import java.util.UUID;

public class CommandServiceResponse
{
  public final UUID eventId;

  @JsonCreator
  public CommandServiceResponse(
      @JsonProperty("eventId") UUID eventId)
  {
    this.eventId = eventId;
  }

}
