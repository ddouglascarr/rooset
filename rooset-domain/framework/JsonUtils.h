#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "framework/uuid.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "enums/ExceptionCode.h"

using namespace std;

namespace rooset {



class JsonUtils
{
public:
  static string serialize(const rapidjson::Document& d);

  static rapidjson::Document parse(const string& json);

  static rapidjson::SchemaDocument parseSchema(const string& json);

  static void validate(
      const rapidjson::SchemaDocument& schema,
      const rapidjson::Document& d);

  static string parseString(const rapidjson::Value& v);

  static rapidjson::Value serializeString(
      const string& s, rapidjson::Document::AllocatorType& allocator);

  static uuid parseUuid(const rapidjson::Value& v);

  static rapidjson::Value serializeUuid(
      const rooset::uuid& id, rapidjson::Document::AllocatorType& allocator);

  template<typename I>
  inline static vector<I> parseArray(
      const rapidjson::Value& a, std::function<I(const rapidjson::Value&)> f)
  {
    vector<I> v;
    for (auto& i : a.GetArray()) {
      v.push_back(f(i));
    } 
    return v;
  }

  template<typename I>
  inline static rapidjson::Value serializeArray(
      const vector<I>& items, 
      std::function<
          rapidjson::Value(const I& i, rapidjson::Document::AllocatorType& allocator)> f,
      rapidjson::Document::AllocatorType& allocator)
  {
    rapidjson::Value v(rapidjson::kArrayType);
    for (auto& item : items) {
      v.PushBack(f(item, allocator), allocator);
    }
    return v;
  }
};



}
