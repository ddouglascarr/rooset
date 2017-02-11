package org.rooset.httpapi.services;

import org.springframework.stereotype.Service;

import java.util.UUID;

@Service
public class IdServiceImpl implements IdService
{
  @Override
  public UUID generateUniqueId()
  {
    return UUID.randomUUID();
  }
}
