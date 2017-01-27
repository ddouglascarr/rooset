package org.rooset.httpapi.repositories;

import org.rooset.httpapi.models.UserModel;
import org.springframework.data.repository.CrudRepository;

public interface UserRepository extends CrudRepository<UserModel, Long>
{
  UserModel findByEmail(String email);
}
