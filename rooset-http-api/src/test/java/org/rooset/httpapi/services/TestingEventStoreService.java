package org.rooset.httpapi.services;

import org.json.JSONObject;

import java.io.IOException;
import java.util.UUID;

public interface TestingEventStoreService
{
  void startTestingEventStore(Process process);
  void killTestingEventStore(Process process);
  JSONObject getLastEventForAggregate(UUID aggregateId) throws IOException;
}
