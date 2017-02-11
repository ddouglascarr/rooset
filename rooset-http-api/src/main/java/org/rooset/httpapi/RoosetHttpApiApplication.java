package org.rooset.httpapi;

import org.json.JSONObject;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.rooset.httpapi.services.CommandService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Profile;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.rooset.httpapi.enums.ExceptionCode;

import java.util.UUID;


@SpringBootApplication
public class RoosetHttpApiApplication {

	private static final Logger log = LoggerFactory.getLogger(RoosetHttpApiApplication.class);

	public static void main(String[] args) {
		SpringApplication.run(RoosetHttpApiApplication.class, args);
	}

  @Autowired
  private PasswordEncoder passwordEncoder;
	@Autowired
  CommandService commandService;

	// Creates a user for development purposes.
	@Profile("clean-dev")
  @Bean
  public CommandLineRunner demo(UserRepository userRepository)
  {
    return (args) -> {
      PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
      UserModel fooUser = new UserModel(
          "foobar", "foo", "bar", "foo@bar.com",
          passwordEncoder.encode("password1"));
      fooUser.setId(UUID.fromString("464b1ebb-32c1-460c-8e9e-333333333333"));
      userRepository.save(fooUser);
    };

  }
}
