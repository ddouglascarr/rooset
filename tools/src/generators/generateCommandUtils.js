const {
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');


module.exports = function(schemas) {
  const ifBlocks = schemas.map((schema) => {
    const msgType = getMsgTypeFromSchema(schema);
    const classname = generateClassnameFromMsgType(msgType);
    return `
      if (msgType == "${msgType}") {
        JsonUtils::validate(${classname}::schema, d);
        const ${classname} cmd(d);
        return commandHandler.evaluate(cmd);
      }`;
  });

  const includeStatements = schemas.map((schema) => {
    const t = getMsgTypeFromSchema(schema);
    return `#include "commands/${generateClassnameFromMsgType(t)}.h"`;
  });

  return `
#pragma once
#include <functional>
#include <memory>
#include "exceptions/CommandEvaluationException.h"
#include "rapidjson/document.h"
#include "framework/MessageUtils.h"
#include "framework/JsonUtils.h"

${includeStatements.join('\n')}

namespace rooset {

  class CommandUtils {
  public:
    template<typename CommandHandler>
    inline static unique_ptr<ProjectEvent<Document>> evaluate(
        CommandHandler& commandHandler,
        const rapidjson::Document& d)
    {
      const string msgType = d["type"].GetString();

      ${ifBlocks.join('\n')}

      throw CommandEvaluationException(
          ExceptionCode::GENERAL_PROJECT_EXCEPTION,
          "There is no ifBlock for " + msgType + ". Run make again, and check the generateCommandUtils script");
    }

  };
}


  `;

};
