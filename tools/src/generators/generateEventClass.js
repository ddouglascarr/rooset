const {
  getTypenameFromRef,
  getMsgTypeFromSchema,
  generateClassnameFromMsgType,
} = require('../utils');
const generateMessageStatements = require('./generateMessageStatements');


module.exports = function(schema, commandConstructorSchema) {
  const messageType = getMsgTypeFromSchema(schema);
  const classTypename = generateClassnameFromMsgType(messageType);
  const payloadProps = schema.properties.payload.properties;
  const {
    declarations,
    stdConstructor,
    jsonConstructor,
    commandConstructor,
    commandConstructorImportStatement,
    serializeStatements,
  } = generateMessageStatements(schema, commandConstructorSchema);

  return `
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "framework/uuid.h"
#include "framework/ProjectEvent.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "framework/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"
${commandConstructorImportStatement}

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class ${classTypename} : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "${messageType}";

  public:
    static const SchemaDocument schema;

    ${declarations.join('\n        ')}

    ${stdConstructor}

    ${jsonConstructor}

    ${commandConstructor}

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

    string getMessageType() const override
    {
      return MESSAGE_TYPE;
    }

    uuid getAggregateId() const override
    {
      return id;
    }

  };

};
  `;
};
