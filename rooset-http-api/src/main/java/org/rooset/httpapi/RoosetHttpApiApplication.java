package org.rooset.httpapi;

import org.rooset.httpapi.models.User;
import org.rooset.httpapi.repositories.UserRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;

@SpringBootApplication
public class RoosetHttpApiApplication {

	private static final Logger log = LoggerFactory.getLogger(RoosetHttpApiApplication.class);

	public static void main(String[] args) {
		SpringApplication.run(RoosetHttpApiApplication.class, args);
	}

	@Bean
  public CommandLineRunner demo(UserRepository userRepository)
  {
    return (args) -> {
      userRepository.save(new User(
          "foo", "bar", "foo@bar.com", "password1"));
      userRepository.save(new User(
          "bing", "bong", "bing@bong.com", "password2"));

      log.info("Users with findAll():");
      for (User user : userRepository.findAll()) {
        log.info(user.getEmail());
      }

    /*  log.info("find foo by email");
      for (User user : userRepository.findByEmail("foo@bar.com")) {
        log.info(user.getFirstName());
        log.info(user.getLastName());
      }
      */
    };
  }
}
