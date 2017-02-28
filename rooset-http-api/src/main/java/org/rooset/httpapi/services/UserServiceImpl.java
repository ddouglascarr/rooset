package org.rooset.httpapi.services;

import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.exceptions.QueryException;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.UUID;

@Service
public class UserServiceImpl implements UserService
{
  @Autowired
  UserRepository userRepository;


  @Override
  public UserModel findOne(UUID id) throws QueryException
  {
    UserModel user = userRepository.findOne(id);
    if (null == user) throw new QueryException(
        ExceptionCode.ITEM_NOT_FOUND_EXCEPTION,
        "No user with id: " + id.toString());
    return user;
  }

  @Override
  public UserModel findOneByUsername(String username)
      throws QueryException
  {
    UserModel user = userRepository.findOneByUsername(username);
    if (null == user) throw new QueryException(
        ExceptionCode.ITEM_NOT_FOUND_EXCEPTION,
        "No user with username: " + username);
    return user;
  }


}
