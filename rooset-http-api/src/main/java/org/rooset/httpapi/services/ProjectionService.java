package org.rooset.httpapi.services;

import org.rooset.httpapi.exceptions.RatkException;

public interface ProjectionService
{
  String query(String uri) throws RatkException;
}
