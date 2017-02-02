
package org.rooset.httpapi.commandcontrollers;

import org.rooset.httpapi.models.UserDetailsImpl;
import org.rooset.httpapi.services.UserDetailsServiceImpl;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import java.util.UUID;

import org.rooset.httpapi.httpcommandrequestbodies.CreateUnitHttpCommandRequestBody;
import org.rooset.httpapi.commands.CreateUnitCommand;
import org.rooset.httpapi.aop.HandleServiceErrors;
import org.rooset.httpapi.exceptions.RatkException;


@RestController
public class CreateUnitHttpCommandRequestController
{


  @HandleServiceErrors
  @RequestMapping(
      value="/units",
      method=RequestMethod.POST)
  public ResponseEntity<CreateUnitCommand> executeCreateUnitCommand(
      @AuthenticationPrincipal UserDetailsImpl user,
      @RequestBody CreateUnitHttpCommandRequestBody requestBody)
      throws RatkException
  {
    CreateUnitCommand cmd = new CreateUnitCommand(
        UUID.randomUUID(), user.getId(), requestBody.name, requestBody.description);

    return new ResponseEntity<>(cmd, HttpStatus.CREATED);
  }



}

