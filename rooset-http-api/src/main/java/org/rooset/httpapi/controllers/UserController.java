package org.rooset.httpapi.controllers;

import org.json.JSONObject;
import org.rooset.httpapi.aop.HandleServiceErrors;
import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.models.UserModel;
import org.rooset.httpapi.services.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.awt.*;
import java.util.UUID;

@RestController
public class UserController
{
  @Autowired
  UserService userService;



  @HandleServiceErrors
  @RequestMapping(
      value="/users/{id}",
      method= RequestMethod.GET,
      produces= MediaType.APPLICATION_JSON_VALUE)
  public ResponseEntity<String> getUser(
      @PathVariable UUID id)
      throws RatkException
  {
    JSONObject json = userService.findOne(id).publicSerialize();
    return new ResponseEntity<>(json.toString(), HttpStatus.OK);
  }



}
