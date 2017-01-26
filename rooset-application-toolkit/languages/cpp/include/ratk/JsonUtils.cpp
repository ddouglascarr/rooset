#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include "ratk/JsonUtils.h"
#include "ratk/IdToolsImpl.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;

namespace rooset {



string rooset::JsonUtils::serialize(const rapidjson::Document& d)
{
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  d.Accept(writer);
  return string(buffer.GetString(), buffer.GetSize());
}



rapidjson::Document rooset::JsonUtils::parse(const string& json)
{
  auto d = rapidjson::Document();
  if (d.Parse(json.c_str(), json.size()).HasParseError()) {
    throw invalid_argument("invalid json");
  }
  return d;
}



rapidjson::SchemaDocument rooset::JsonUtils::parseSchema(const string& json)
{
  auto sd = parse(json);
  return rapidjson::SchemaDocument(sd);
}



void rooset::JsonUtils::validate(
    const rapidjson::SchemaDocument& schema,
    const rapidjson::Document& d)
{
  rapidjson::SchemaValidator validator(schema);
  if (!d.Accept(validator)) {
    rapidjson::StringBuffer sb;
    validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
    string bMsg = "Invalid schema: " + string(sb.GetString()) + " in " +
        string(d["type"].GetString());
    sb.Clear();
    validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);

    throw invalid_argument(
        string("json does not comply with schema\n") +
        string(validator.GetInvalidSchemaKeyword()) + "\n" +
        bMsg + "\n" +
        "Invalid document" + string(sb.GetString()) +"\n");
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



void rooset::JsonUtils::forEach(
      const rapidjson::Value& array,
      std::function<void(const rapidjson::Value&)> f)
{
  if (!array.IsArray()) throw std::invalid_argument(
      "JsonUtils::forEach requires an array, but it didnt get one");

  for (auto& item : array.GetArray()) {
    f(item);
  }
}



void rooset::JsonUtils::forEachReverse(
    const rapidjson::Value& array_,
    std::function<void(const rapidjson::Value&)> f)
{
  if (!array_.IsArray()) throw std::invalid_argument(
      "JsonUtils::forEach requires an array, but it didnt get one");

  auto array = array_.GetArray();
  for (rapidjson::SizeType it = array.Size(); it > 0; it--) {
    f(array[it - 1]);
  }
}


}



