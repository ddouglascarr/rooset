package org.rooset.httpapi.config;

import org.rooset.httpapi.services.UserDetailsServiceImpl;
import org.springframework.context.annotation.Bean;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.web.authentication.SimpleUrlAuthenticationFailureHandler;

@Configuration
@EnableWebSecurity
public class WebSecurityConfig extends WebSecurityConfigurerAdapter
{
  @Autowired
  private UserDetailsServiceImpl userDetailsService;

  @Autowired
  RestAuthenticationEntryPoint authenticationEntryPoint;

  @Autowired
  RestAuthenticationSuccessHandler authenticationSuccessHandler;

  @Autowired
  RestAuthenticationFailureHandler authenticationFailureHandler;

  @Autowired
  public void configAuthentication(AuthenticationManagerBuilder auth) throws Exception
  {
    auth.userDetailsService(userDetailsService).passwordEncoder(passwordEncoder());
  }

  @Override
  protected void configure(HttpSecurity http) throws Exception
  {
    http.csrf().disable();
    http.authorizeRequests()
        .antMatchers("/unit-url-parameter-names/**").permitAll()
        .antMatchers("/units/**").permitAll()
        .antMatchers("/users/{id}").permitAll()
        .anyRequest().authenticated();

    http.exceptionHandling().authenticationEntryPoint(authenticationEntryPoint);
    http.httpBasic()
        .and()
        .formLogin()
            .successHandler(authenticationSuccessHandler)
            .failureHandler(authenticationFailureHandler)
        .and()
        .logout()
        .permitAll();
  }

  @Bean(name="passwordEncoder")
  public PasswordEncoder passwordEncoder() { return new BCryptPasswordEncoder(); }
}
