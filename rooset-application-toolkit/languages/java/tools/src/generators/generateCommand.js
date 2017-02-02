const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const generateMessageStatements = require('./generateMessageStatements');


module.exports = function(javaBasePackage, schema) {
  const messageType = getMsgTypeFromSchema(schema);
  const className = generateClassnameFromMsgType(messageType);
  const {
    declarations,
    stdConstructor,
  } = generateMessageStatements(schema);
  
  return `
package ${javaBasePackage}.commands;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;


public class ${className}
{
  ${declarations.join('\n  ')}


  @JsonCreator
  ${stdConstructor}
}

`;
}
