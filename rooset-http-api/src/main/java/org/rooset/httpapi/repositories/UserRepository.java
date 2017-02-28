package org.rooset.httpapi.repositories;

import org.rooset.httpapi.models.UserModel;
import org.springframework.data.repository.CrudRepository;

import java.util.UUID;

public interface UserRepository extends CrudRepository<UserModel, UUID>
{
  UserModel findOneByEmail(String email);
  UserModel findOneByUsername(String username);
}
