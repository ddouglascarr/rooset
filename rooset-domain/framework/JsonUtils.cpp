#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;

namespace rooset {



  unique_ptr<string> rooset::JsonUtils::serialize(const rapidjson::Document& d)
  {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    return make_unique<string>(buffer.GetString(), buffer.GetSize());
  }



  unique_ptr<rapidjson::Document> rooset::JsonUtils::parse(const string& json)
  {
    auto d = make_unique<rapidjson::Document>();
    if (d->Parse(json.c_str(), json.size()).HasParseError()) {
      throw invalid_argument("invalid json");
    }
    return d;
  }



  unique_ptr<rapidjson::SchemaDocument> rooset::JsonUtils::parseSchema(const string& json)
  {
    auto sd = parse(json);
    return make_unique<rapidjson::SchemaDocument>(*sd);
  }



  void rooset::JsonUtils::validate(const rapidjson::SchemaDocument& schema, const rapidjson::Document& d)
  {
    rapidjson::SchemaValidator validator(schema);
    if (!d.Accept(validator)) {
      std::cout << validator.GetInvalidSchemaKeyword() << "\n";
      rapidjson::StringBuffer sb;
      validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
      cout << "Invalid schema: " <<  sb.GetString() << " in " << d["type"].GetString() << "\n";
      sb.Clear();
      validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
      cout << "Invalid document" << sb.GetString() << "\n";
      throw invalid_argument("json does not comply with schema");
    }
  }


  
  string rooset::JsonUtils::parseString(const rapidjson::Value& v)
  {
    return v.GetString();
  } 



  rapidjson::Value rooset::JsonUtils::serializeString(
      const string& s, rapidjson::Document::AllocatorType& allocator)
  {
    rapidjson::Value v;
    v.SetString(s.c_str(), s.size(), allocator);
    return v;
  }



  uuid rooset::JsonUtils::parseUuid(const rapidjson::Value& v)
  {
    IdToolsImpl idTools;
    if (v.IsNull()) {
      return idTools.generateNilId();
    }
    return idTools.parse(JsonUtils::parseString(v));
  }



  rapidjson::Value rooset::JsonUtils::serializeUuid(
      const rooset::uuid& id, rapidjson::Document::AllocatorType& allocator)
  {
    IdToolsImpl idTools;
    rapidjson::Value v; // null

    if (!id.is_nil()) {
      auto s = idTools.serialize(id);
      v = JsonUtils::serializeString(s, allocator);
    }

    return v;
  }



}



