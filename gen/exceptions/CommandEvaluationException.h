
#pragma once
#include <string>
#include <memory>
#include "framework/ProjectException.h"
#include "framework/JsonUtils.h"
#include "enums/ExceptionCode.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/schema.h"

using namespace std;
using namespace rapidjson;

namespace rooset {

  class CommandEvaluationException : public ProjectException<Document>
  {
  private:
    const string msg;
    const ExceptionCode exceptionCode;
    const string MESSAGE_TYPE = "COMMAND_EVALUATION_EXCEPTION";
  public:
    static const unique_ptr<SchemaDocument> schema;

    CommandEvaluationException(ExceptionCode code, string msg) :
        ProjectException<Document>(msg),
        msg(msg),
        exceptionCode(code)
    {}

    CommandEvaluationException(const Document& d) :
        ProjectException<Document>(string(
          d["payload"]["message"].GetString(),
          d["payload"]["message"].GetStringLength())),
        msg(string(
          d["payload"]["message"].GetString(),
          d["payload"]["message"].GetStringLength())),
        exceptionCode(JsonUtils::parseExceptionCode(string(
          d["payload"]["code"].GetString(),
          d["payload"]["code"].GetStringLength())))
    {}

    unique_ptr<Document> serialize() const override
    {
      auto d = make_unique<Document>();
      d->SetObject();

      Value payload;
      payload.SetObject();

      Value messageTypeValue;
      messageTypeValue.SetString(
          MESSAGE_TYPE.c_str(), MESSAGE_TYPE.size());
      d->AddMember("type", messageTypeValue, d->GetAllocator());

      Value isErrValue;
      isErrValue.SetBool(true);
      d->AddMember("error", isErrValue, d->GetAllocator());

      Value messageValue;
      messageValue.SetString(msg.c_str(), msg.size(), d->GetAllocator());
      payload.AddMember("message", messageValue, d->GetAllocator());

      Value codeValue;
      string codeStr = JsonUtils::serializeExceptionCode(exceptionCode);
      codeValue.SetString(codeStr.c_str(), codeStr.size(), d->GetAllocator());
      payload.AddMember("code", codeValue, d->GetAllocator());

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    ExceptionCode getExceptionCode() const override
    {
      return exceptionCode;
    }
  };
}