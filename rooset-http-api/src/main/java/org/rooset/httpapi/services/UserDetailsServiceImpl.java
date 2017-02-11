package org.rooset.httpapi.services;

import org.rooset.httpapi.models.UserDetailsImpl;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class UserDetailsServiceImpl implements UserDetailsService
{
  @Autowired
  private final UserRepository userRepository;

  @Autowired
  UserDetailsServiceImpl(UserRepository userRepository)
  {
    this.userRepository = userRepository;
  }

  @Override
  public UserDetailsImpl loadUserByUsername(String login) throws UsernameNotFoundException
  {
    UserModel user;

    user = userRepository.findOneByUsername(login);
    if (null == user) {
      throw new UsernameNotFoundException("No user present with login: " + login);
    }

    List<String> userRoles = new ArrayList<>();
    userRoles.add("ROLE_USER");
    UserDetailsImpl userDetails =  new UserDetailsImpl(user, userRoles);
    return userDetails;
  }

}
