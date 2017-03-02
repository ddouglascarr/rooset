package org.rooset.httpapi.models;

import org.json.JSONObject;

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

  @Column(unique = true) private String username;
  private String displayName;
  private String firstName;
  private String lastName;
  @Column(unique = true) String email;
  private String password;

  protected UserModel() {}

  public UserModel(String username, String displayName, String firstName, String lastName, String email, String password)
  {
    this.id = UUID.randomUUID();
    this.username = username;
    this.displayName = displayName;
    this.firstName = firstName;
    this.lastName = lastName;
    this.email = email;
    this.password = password;
  }

  // used by UserDetailsImpl
  public UserModel(UserModel user)
  {
    this.id = user.id;
    this.username = user.username;
    this.displayName = user.displayName;
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

  public String getUsername()
  {
    return username;
  }

  public void setUsername(String username)
  {
    this.username = username;
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

  public String getDisplayName()
  {
    return displayName;
  }

  public void setDisplayName(String displayName)
  {
    this.displayName = displayName;
  }

  // for public access. No email or password
  public JSONObject publicSerialize()
  {
    JSONObject out = new JSONObject()
        .put("id", id.toString())
        .put("username", username)
        .put("displayName", displayName);
    return out;
  }
}
