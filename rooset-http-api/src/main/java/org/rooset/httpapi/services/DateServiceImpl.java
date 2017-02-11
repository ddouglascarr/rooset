package org.rooset.httpapi.services;

import org.springframework.stereotype.Service;

import java.time.ZoneOffset;
import java.time.ZonedDateTime;

@Service
public class DateServiceImpl implements DateService
{
  @Override
  public long getNow()
  {
    ZonedDateTime now = ZonedDateTime.now(ZoneOffset.UTC);
    return now.toEpochSecond();
  }
}
