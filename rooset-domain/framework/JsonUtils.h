#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
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

    template<typename I>
    inline static vector<I> parseArray(const rapidjson::Value& a, std::function<I(const rapidjson::Value&)> f)
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
        std::function<rapidjson::Value(const I& i)> f,
        rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value v(rapidjson::kArrayType);
      for (auto& item : items) {
        v.PushBack(f(item), allocator);
      }
      return v;
    }
  };
}