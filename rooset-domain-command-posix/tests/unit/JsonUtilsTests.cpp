#include "gtest/gtest.h"

#include <string>
#include "ratk/JsonUtils.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rooset;

namespace roosetJsonUtilsTests {

  TEST(JsonUtils, parseArray)
  {
    const string input = R"json({
      "items": [
        "foo", 
        "bar", 
        "bing",
        "bong"
      ]
    })json";

    const auto doc = JsonUtils::parse(input);
    const auto out = JsonUtils::parseArray<string>(doc["items"],
        [](const rapidjson::Value& v) { return v.GetString(); });
    const vector<string> expected = { "foo", "bar", "bing", "bong" };
    EXPECT_EQ(out, expected);
  }

}

TEST(JsonUtils, serializeArray)
{
  const string expectedStr = R"json({
      "items": [
        "foo", 
        "bar", 
        "bing",
        "bong"
      ]
    })json";
  const auto expected = JsonUtils::parse(expectedStr);
  const vector<string> toSerialize = { "foo", "bar", "bing", "bong" };

  rapidjson::Document d;
  d.SetObject();
  auto items = JsonUtils::serializeArray<string>(
    toSerialize,
    [](const string& item, auto& allocator) {
        rapidjson::Value v;
        v.SetString(item.c_str(), item.size(), allocator);
        return v; },
    d.GetAllocator());
  
  d.AddMember("items", items, d.GetAllocator());
  EXPECT_EQ(d, expected);
}



TEST(JsonUtils, forEach)
{
  const string sJson = u8R"json({
    "items": [
      "foo",
      "bar",
      "bing",
      "bong"
    ]
  })json";

  auto doc = JsonUtils::parse(sJson);
  const vector<string> expected { "foo", "bar", "bing", "bong" };
  vector<string> result;

  JsonUtils::forEach(doc["items"],
      [&](const rapidjson::Value& v) { result.push_back(v.GetString()); });

  EXPECT_EQ(result, expected);
}



TEST(JsonUtils, forEachReverse)
{
  const string sJson = u8R"json({
    "items": [
      "foo",
      "bar",
      "bing",
      "bong"
    ]
  })json";

  auto doc = JsonUtils::parse(sJson);
  const vector<string> expected { "bong", "bing", "bar", "foo" };
  vector<string> result;

  JsonUtils::forEachReverse(doc["items"],
      [&](const rapidjson::Value& v) { result.push_back(v.GetString()); });

  EXPECT_EQ(result, expected);
}



TEST(JsonUtils, map)
{
  const string sJson = u8R"json({
    "items": [
      "foo",
      "bar",
      "bing",
      "bong"
    ]
  })json";

  auto doc = JsonUtils::parse(sJson);
  const vector<string> expected { "foo", "bar", "bing", "bong" };

  auto result = JsonUtils::map<string>(doc["items"],
    [&](const rapidjson::Value& v) { return JsonUtils::parseString(v); });

  EXPECT_EQ(result, expected);
}
