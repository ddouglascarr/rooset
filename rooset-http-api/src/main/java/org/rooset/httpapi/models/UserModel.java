package org.rooset.httpapi.models;

import java.util.UUID;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.UniqueConstraint;

@Entity
public class UserModel
{

  @Id
  private UUID id;

  private String firstName;
  private String lastName;
  @Column(unique = true) String email;
  private String password;

  protected UserModel() {}

  public UserModel(String firstName, String lastName, String email, String password)
  {
    this.id = UUID.randomUUID();
    this.firstName = firstName;
    this.lastName = lastName;
    this.email = email;
    this.password = password;
  }

  // used by UserDetailsImpl
  public UserModel(UserModel user)
  {
    this.id = user.id;
    this.firstName = user.firstName;
    this.lastName = user.lastName;
    this.email = user.email;
    this.password = user.password;
  }

  public UUID getId()
  {
    return id;
  }

  public void setId(UUID id)
  {
    this.id = id;
  }

  public String getFirstName()
  {
    return firstName;
  }

  public void setFirstName(String firstName)
  {
    this.firstName = firstName;
  }

  public String getLastName()
  {
    return lastName;
  }

  public void setLastName(String lastName)
  {
    this.lastName = lastName;
  }

  public String getEmail()
  {
    return email;
  }

  public void setEmail(String email)
  {
    this.email = email;
  }

  public String getPassword()
  {
    return password;
  }

  public void setPassword(String password)
  {
    this.password = password;
  }
}
