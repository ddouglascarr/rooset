#pragma once
#include <memory>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "enums/ExceptionCode.h"

using namespace std;

namespace rooset {
  class JsonUtils
  {
  public:
    static unique_ptr<string> serialize(const rapidjson::Document& d);
    static unique_ptr<rapidjson::Document> parse(const string& json);
    static unique_ptr<rapidjson::SchemaDocument> parseSchema(const string& json);
    static void validate(const rapidjson::SchemaDocument& schema, const rapidjson::Document& d);
    static ExceptionCode parseExceptionCode(const string& code);
    static string serializeExceptionCode(ExceptionCode code);
  };
}