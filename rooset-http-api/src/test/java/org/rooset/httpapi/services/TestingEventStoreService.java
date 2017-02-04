package org.rooset.httpapi.services;

import org.apache.commons.exec.ExecuteWatchdog;
import org.json.JSONObject;
import java.io.IOException;
import java.util.UUID;

public interface TestingEventStoreService
{
  ExecuteWatchdog startTestingEventStore() throws IOException;
  void killTestingEventStore(Process process) throws IOException;
  JSONObject getLastEventForAggregate(UUID aggregateId) throws IOException;
}
