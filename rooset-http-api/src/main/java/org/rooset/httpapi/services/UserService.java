package org.rooset.httpapi.services;

import org.rooset.httpapi.exceptions.QueryException;
import org.rooset.httpapi.models.UserModel;

import java.util.UUID;

public interface UserService
{
  UserModel findOne(UUID id) throws QueryException;
  UserModel findOneByUsername(String username) throws QueryException;
}
