package org.rooset.httpapi.controllers;

import org.rooset.httpapi.aop.HandleServiceErrors;
import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.models.UserDetailsImpl;
import org.rooset.httpapi.services.SearchService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.UUID;

@RestController
public class SearchController
{
  @Autowired
  SearchService searchService;



  @HandleServiceErrors
  @RequestMapping(
      value="/units/{unitId}/issues/_search",
      method= RequestMethod.POST,
      produces= MediaType.APPLICATION_JSON_VALUE)
  public ResponseEntity<String> SearchIssuesByUnit(
      @PathVariable UUID unitId,
      @RequestBody String searchQuery)
      throws RatkException
  {
    String resp = searchService.performSearch(
        "units", unitId.toString(), "issues", searchQuery);
    return new ResponseEntity<String>(resp, HttpStatus.OK);
  }



}
