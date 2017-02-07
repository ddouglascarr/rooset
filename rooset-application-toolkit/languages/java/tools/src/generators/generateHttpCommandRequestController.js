const { each, map } = require('lodash');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const {
  getPathVariables,
} = require('../../../../../ratk-declarations-utils');
const typenameMap = require('../templates/typenameMap');
const generateMessageStatements = require('./generateMessageStatements');

module.exports = function(javaBasePackage, declaration, commandSchema) {
  const messageType = `${declaration.type}_CONTROLLER`;
  const className = generateClassnameFromMsgType(messageType);
  const requestBodyMessageType = `${declaration.type}_BODY`;
  const requestBodyClassName = generateClassnameFromMsgType(requestBodyMessageType);
  const commandMessageType = getMsgTypeFromSchema(commandSchema);
  const commandClassName = generateClassnameFromMsgType(commandMessageType);
  
  const commandSchemaPayloadProps = commandSchema.properties.payload.properties;
  const commandSchemaPropTypes = map(commandSchemaPayloadProps, (ref, v) => getTypenameFromRef(ref));
  const commandSchemaPropVariables = Object.keys(commandSchema.properties.payload.properties);

  const runtimeGenerate = (v) => {
    const typename = getTypenameFromRef(commandSchemaPayloadProps[v])
    if (typename === 'uuid') {
      return 'idService.generateUniqueId()';
    }
    throw new Error(`no runtime generation handling for ${typename} ${v}`);
  }

  const pathVariables = getPathVariables(declaration.uri);
  let hasRequestBody = false;
  const commandConstructorParams = commandSchemaPropVariables.map((v) => {
    if (declaration.userIdMapping === v) {
      return 'user.getId()';
    } else if (pathVariables.indexOf(v) !== -1) {
      return v;
    } else if (declaration.generate && declaration.generate.indexOf(v) !== -1) {
      return runtimeGenerate(v)
    } else {
      hasRequestBody = true;
      return `requestBody.${v}`;
    }
  });
  const variableParams = pathVariables.map(
      (v) => `@PathVariable ${typenameMap[getTypenameFromRef(commandSchemaPayloadProps[v])]} ${v}`);
  variableParams.push(`@AuthenticationPrincipal UserDetailsImpl user`);
  if (hasRequestBody) {
    variableParams.push(`@RequestBody ${requestBodyClassName} requestBody`);
  }




  return `
package ${javaBasePackage}.commandcontrollers;

import org.rooset.httpapi.models.UserDetailsImpl;
import org.springframework.beans.factory.annotation.Autowired;
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

import ${javaBasePackage}.httpcommandrequestbodies.${requestBodyClassName};
import ${javaBasePackage}.commands.${commandClassName};
import org.rooset.httpapi.aop.HandleServiceErrors;
import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.services.CommandService;
import org.rooset.httpapi.models.CommandServiceResponse;
import org.rooset.httpapi.services.IdService;


@RestController
public class ${className}
{

  @Autowired
  CommandService commandService;

  @Autowired
  IdService idService;

  @HandleServiceErrors
  @RequestMapping(
      value="${declaration.uri}",
      method=RequestMethod.${declaration.method})
  public ResponseEntity<CommandServiceResponse> execute${commandClassName}(
      ${variableParams.join(',\n    ')})
      throws RatkException
  {
    ${commandClassName} cmd = new ${commandClassName}(
        ${commandConstructorParams.join(', ')});

    CommandServiceResponse resp = commandService.execute(cmd.serialize());
    return new ResponseEntity<>(resp, HttpStatus.CREATED);
  }



}

`;

}
