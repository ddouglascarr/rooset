#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include "framework/JsonUtils.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;

namespace rooset {

  unique_ptr<string> JsonUtils::serialize(const rapidjson::Document& d)
  {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    return make_unique<string>(buffer.GetString(), buffer.GetSize());
  }

  unique_ptr<rapidjson::Document> JsonUtils::parse(const string& json)
  {
    auto d = make_unique<rapidjson::Document>();
    if (d->Parse(json.c_str(), json.size()).HasParseError()) {
      throw invalid_argument("invalid json");
    }
    return d;
  }

  unique_ptr<rapidjson::SchemaDocument> JsonUtils::parseSchema(const string& json)
  {
    auto sd = parse(json);
    return make_unique<rapidjson::SchemaDocument>(*sd);
  }

  void JsonUtils::validate(const rapidjson::SchemaDocument& schema, const rapidjson::Document& d)
  {
    rapidjson::SchemaValidator validator(schema);
    if (!d.Accept(validator)) {
      std::cout << validator.GetInvalidSchemaKeyword() << "\n";
      rapidjson::StringBuffer sb;
      validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
      cout << "Invalid schema: " <<  sb.GetString() << "\n";
      sb.Clear();
      validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
      cout << "Invalid document" << sb.GetString() << "\n";
      throw invalid_argument("json does not comply with schema");
    }
  }
}