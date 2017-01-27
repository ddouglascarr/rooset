package org.rooset.httpapi.repositories;

import org.rooset.httpapi.models.User;
import org.springframework.data.repository.CrudRepository;

import java.util.List;
import java.util.UUID;

public interface UserRepository extends CrudRepository<User, Long>
{
  // List<User> findByEmail(String email);
}
