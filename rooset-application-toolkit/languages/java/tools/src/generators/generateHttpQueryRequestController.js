const { map, reduce } = require('lodash');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const {
  applyUriTemplate,
} = require('../../../../../ratk-declarations-utils');
const typenameMap = require('../templates/typenameMap');


module.exports = (declaration, config) => {
  const { javaBasePackage } = config;
  const messageType = `${declaration.type}_HTTP_QUERY_CONTROLLER`;
  const className = generateClassnameFromMsgType(messageType);

  const variableParams = map(declaration.req, (v, k) => {
    return `@PathVariable ${typenameMap[v.type]} ${k}`;
  });

  const projectionUriTmplConfig = reduce(declaration.req, (r, v, k) => {
    r[k] = `" + ${k}.toString() + "`;
    return r;
  }, {});
  const projectionUri = `"/projection/${declaration.type}${applyUriTemplate(declaration.projectionUri, projectionUriTmplConfig)}"`;

  return `
package ${javaBasePackage}.querycontrollers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.http.MediaType;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import ${javaBasePackage}.aop.HandleServiceErrors;
import ${javaBasePackage}.exceptions.RatkException;
import ${javaBasePackage}.services.ProjectionService;


@RestController
public class ${className}
{

  @Autowired
  ProjectionService projectionService;



  @HandleServiceErrors
  @RequestMapping(
      value="${declaration.httpUri}",
      method=RequestMethod.GET,
      produces=MediaType.APPLICATION_JSON_VALUE)
  public ResponseEntity<String> executeQuery(
      ${variableParams.join(',\n    ')})
      throws RatkException
  {
    String reducerResp = projectionService.query(
        ${projectionUri});
    return new ResponseEntity<>(reducerResp, HttpStatus.OK);
  }



}
`;



};
