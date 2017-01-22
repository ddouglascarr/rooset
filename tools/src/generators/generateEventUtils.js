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
        const ${classname} evt(d);
        MessageUtils::applyEvent<Aggregate, ${classname}>(aggregate, evt, onMethodMissing);
        return;
      }`;
  });

  const includeStatements = schemas.map((schema) => {
    const t = getMsgTypeFromSchema(schema);
    return `#include "events/${generateClassnameFromMsgType(t)}.h"`;
  });


  return `
#pragma once
#include <functional>
#include "exceptions/CommandEvaluationException.h"
#include "rapidjson/document.h"
#include "framework/MessageUtils.h"
#include "framework/JsonUtils.h"

${includeStatements.join('\n')}

namespace rooset {

  class EventUtils {
  public:
    template<typename Aggregate>
    static void applyEvent(
        unique_ptr<Aggregate>& aggregate,
        const rapidjson::Document& d,
        std::function<void()> onMethodMissing)
    {
      const string msgType = d["type"].GetString();

      ${ifBlocks.join('\n')}

      throw CommandEvaluationException(
          ExceptionCode::GENERAL_PROJECT_EXCEPTION,
          "There is no ifBlock for " + msgType + ". Run make again, and check the generateEventUtils script");
    }

  };
}


  `;
}
