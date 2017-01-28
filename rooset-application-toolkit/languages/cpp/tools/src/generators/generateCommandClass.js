const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const generateMessageStatements = require('./generateMessageStatements');


module.exports = function(schema) {
  const messageType = getMsgTypeFromSchema(schema);
  const classTypename = generateClassnameFromMsgType(messageType);
  const {
    declarations,
    stdConstructor,
    jsonConstructor,
    serializeStatements
  } = generateMessageStatements(schema);

  return `
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "ratk/ProjectCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "ratk/IdUtils.h"
#include "ratk/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class ${classTypename} : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "${messageType}";

  public:
    static const SchemaDocument schema;

    ${declarations.join('\n        ')}

    ${stdConstructor}

    ${jsonConstructor}

    unique_ptr<Document> serialize() const override
    {
      auto d = make_unique<Document>();
      d->SetObject();

      Value payload;
      payload.SetObject();

      Value messageTypeValue;
      messageTypeValue.SetString(MESSAGE_TYPE.c_str(),
          MESSAGE_TYPE.size(), d->GetAllocator());
      d->AddMember("type", messageTypeValue, d->GetAllocator());

      ${serializeStatements.join('\n')}

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};`;

}