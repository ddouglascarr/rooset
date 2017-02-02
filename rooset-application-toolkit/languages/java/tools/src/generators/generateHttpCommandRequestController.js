const { each, map } = require('lodash');
const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const {
  getPathVariables,
} = require('../../../../../ratk-declarations-utils');
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

  const pathVariables = getPathVariables(declaration.uri);
  const variableParams = pathVariables.map(
      (v) => `@PathVariable ${getTypenameFromRef(commandSchemaPayloadProps[v])} ${v}`);
  variableParams.push(`@AuthenticationPrincipal UserDetailsImpl user`);
  variableParams.push(`@RequestBody ${requestBodyClassName} requestBody`);

  const runtimeGenerate = (v) => {
    const typename = getTypenameFromRef(commandSchemaPayloadProps[v])
    if (typename === 'uuid') {
      return 'UUID.randomUUID()';
    }
    throw new Error(`no runtime generation handling for ${typename} ${v}`);
  }

  const commandConstructorParams = commandSchemaPropVariables.map((v) => {
    if (declaration.userIdMapping === v) {
      return 'user.getId()';
    } else if (pathVariables.indexOf(v) !== -1) {
      return v;
    } else if (declaration.generate.indexOf(v) !== -1) {
      return runtimeGenerate(v)
    } else {
      return `requestBody.${v}`;
    }
  });


  return `
package ${javaBasePackage}.commandcontrollers;

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

import ${javaBasePackage}.httpcommandrequestbodies.${requestBodyClassName};
import ${javaBasePackage}.commands.${commandClassName};


@RestController
public class ${className}
{


  @RequestMapping(
      value="${declaration.uri}",
      method=RequestMethod.${declaration.method})
  public ResponseEntity<${commandClassName}> execute${commandClassName}(
      ${variableParams.join(',\n      ')})
  {
    ${commandClassName} cmd = new ${commandClassName}(
        ${commandConstructorParams.join(', ')});

    return new ResponseEntity<>(cmd, HttpStatus.CREATED);
  }



}

`;

}
