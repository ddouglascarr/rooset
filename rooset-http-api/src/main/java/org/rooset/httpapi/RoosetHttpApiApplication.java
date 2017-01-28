package org.rooset.httpapi;

import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.repositories.UserRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.rooset.httpapi.generated.enums.ExceptionCode;


@SpringBootApplication
public class RoosetHttpApiApplication {

	private static final Logger log = LoggerFactory.getLogger(RoosetHttpApiApplication.class);

	public static void main(String[] args) {
		SpringApplication.run(RoosetHttpApiApplication.class, args);
	}

  @Autowired
  private PasswordEncoder passwordEncoder;

	@Bean
  public CommandLineRunner demo(UserRepository userRepository)
  {
    return (args) -> {
      PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
      userRepository.save(new UserModel(
          "foo", "bar", "foo@bar.com",
          passwordEncoder.encode("password1")));
      userRepository.save(new UserModel(
          "bing", "bong", "bing@bong.com",
          passwordEncoder.encode("password2")));

      log.info("Users with findAll():");
      for (UserModel userModel : userRepository.findAll()) {
        log.info(userModel.getEmail());
        log.info(userModel.getPassword());
      }

     log.info("find foo by email");
     UserModel user = userRepository.findOneByEmail("foo@bar.com");

     if (null != user) {
       log.info(user.getFirstName());
       log.info(user.getLastName());
     }

     ExceptionCode r = getExceptionCode("a");
     if (ExceptionCode.CONFLICT_EXCEPTION.equals(r)) {
      log.info("ExceptionCode compiles");
      }
    };
  }

  private ExceptionCode getExceptionCode(String c) {
	  if (c == "a") {
	    return ExceptionCode.CONFLICT_EXCEPTION;
    }
    return ExceptionCode.ITEM_NOT_FOUND_EXCEPTION;
  }
}
