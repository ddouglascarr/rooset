package org.rooset.httpapi.controllers;

import org.json.JSONObject;
import org.rooset.httpapi.aop.HandleServiceErrors;
import org.rooset.httpapi.commands.GrantPrivilegeCommand;
import org.rooset.httpapi.enums.ExceptionCode;
import org.rooset.httpapi.exceptions.CommandEvaluationException;
import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.httpcommandrequests.GrantPrivilegeHttpCommandRequest;
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

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@RestController
public class EchoController
{



  @HandleServiceErrors
  @RequestMapping(
      value="/echo/{val}",
      method= RequestMethod.GET)
  public ResponseEntity<Map<String, String>> getEcho(
      @AuthenticationPrincipal UserDetailsImpl user,
      @PathVariable String val)
      throws RatkException
  {
    HashMap<String, String> resp = new HashMap<>();
    resp.put("you", user.getUsername());
    resp.put("echo", val);
    resp.put("id", user.getId().toString());
    throw new CommandEvaluationException(ExceptionCode.ITEM_NOT_FOUND_EXCEPTION, "Fudge");
    // return new ResponseEntity<>(resp, HttpStatus.OK);
  }



  @RequestMapping(
      value="/units/{unitId}/members",
      method=RequestMethod.POST)
  public ResponseEntity<String> grantPrivilege(
      @AuthenticationPrincipal UserDetailsImpl user,
      @PathVariable UUID unitId,
      @RequestBody GrantPrivilegeHttpCommandRequest reqBody)
  {
    final GrantPrivilegeCommand cmd = new GrantPrivilegeCommand(
        unitId, user.getId(), reqBody.memberId, reqBody.pollingRight, reqBody.votingRight,
        reqBody.initiativeRight, reqBody.managementRight, reqBody.weight);

    return new ResponseEntity<>(cmd.serialize().toString(), HttpStatus.CREATED);
  }
}
