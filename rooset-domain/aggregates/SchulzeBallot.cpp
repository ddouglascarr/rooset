#include <vector>
#include "SchulzeBallot.h"
#include "framework/JsonUtils.h"



rooset::SchulzeBallot::SchulzeBallot(
    const vector<vector<uuid>>& approve,
    const vector<uuid>& inAbstain,
    const vector<vector<uuid>>& disapprove) :
    approve(approve),
    abstain(inAbstain),
    disapprove(disapprove)
{
  schulzeRanking.insert(schulzeRanking.end(), approve.begin(), approve.end());
  auto abstain = inAbstain;
  abstain.push_back(idTools.generateNilId());
  schulzeRanking.push_back({ abstain });
  schulzeRanking.insert(schulzeRanking.end(), disapprove.begin(), disapprove.end());
}
  


rooset::SchulzeBallot rooset::SchulzeBallot::parse(const rapidjson::Value& v)
{
  auto parseUuidArray = [](const rapidjson::Value& v) {
    return JsonUtils::parseArray<uuid>(v, 
      [](const rapidjson::Value& v) {  
        return JsonUtils::parseUuid(v); 
      });
  };
  auto abstain = parseUuidArray(v["abstain"]);
  auto approve = JsonUtils::parseArray<vector<uuid>>(v["approve"],
    [&parseUuidArray](const rapidjson::Value& v) {
      return parseUuidArray(v);
    });
  auto disapprove = JsonUtils::parseArray<vector<uuid>>(v["disapprove"],
    [&parseUuidArray](const rapidjson::Value& v) {
      return parseUuidArray(v);
    });
    
  return SchulzeBallot(approve, abstain, disapprove);
}



rapidjson::Value rooset::SchulzeBallot::serialize(
    const SchulzeBallot& b, rapidjson::Document::AllocatorType& allocator)
{
  auto serializeUuidArray = [](const vector<uuid>& ids, auto& allocator) {
    return JsonUtils::serializeArray<uuid>(ids, [](const uuid& id, auto& allocator) {
      return JsonUtils::serializeUuid(id, allocator); 
    }, allocator);
  };

  IdToolsImpl idTools;
  auto abstainBallot = b.abstain;
  auto statusQuoPosition = find(
      abstainBallot.begin(), abstainBallot.end(), idTools.generateNilId());
  if (statusQuoPosition != abstainBallot.end()) abstainBallot.erase(statusQuoPosition);
  auto abstain = serializeUuidArray(abstainBallot, allocator);
  auto approve = JsonUtils::serializeArray<vector<uuid>>(b.approve,
      [&serializeUuidArray](const vector<uuid> ids, rapidjson::Document::AllocatorType& allocator) {
        return serializeUuidArray(ids, allocator);
      }, allocator);
  auto disapprove = JsonUtils::serializeArray<vector<uuid>>(b.disapprove,
      [&serializeUuidArray](const vector<uuid> ids, auto& allocator) {
        return serializeUuidArray(ids, allocator);
      }, allocator);

  rapidjson::Value v(rapidjson::kObjectType); 
  v.AddMember("approve", approve, allocator);
  v.AddMember("abstain", abstain, allocator);
  v.AddMember("disapprove", disapprove, allocator);
  return v;
}
