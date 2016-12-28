#include "gmock/gmock.h"

#include <vector>
#include <memory>
#include <tuple>
#include "rapidjson/document.h"
#include "aggregates/UnitAggregate.h"
#include "aggregates/IssueAggregate.h"
#include "events/EventUtils.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "aggregates/VoteUtils.h"

using namespace std;
using namespace rooset;

namespace roosetVoteUtilsTests {

  TEST(VoteUtils, calcSupportWeight)
  {
  // pasted from supportCalculationTests.cpp
  vector<unique_ptr<rapidjson::Document>> givenEvents;

  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 3
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 4
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));

    IdToolsImpl idTools;

    // apply events to aggregates
    unique_ptr<UnitAggregate> unit = nullptr;
    unique_ptr<IssueAggregate> issue = nullptr;
    for (const auto& e : givenEvents) {
      EventUtils::applyEvent<UnitAggregate>(unit, *e, []() {});
      EventUtils::applyEvent<IssueAggregate>(issue, *e, []() {});
    }

    auto supportWeightA = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightC = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightF = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-ffffffffffff"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightE = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    EXPECT_EQ(supportWeightA, 3);
    EXPECT_EQ(supportWeightC, 3);
    EXPECT_EQ(supportWeightF, 2);
    EXPECT_EQ(supportWeightE, 1);
  }

  TEST(VoteUtils, calcTotalVoteWeight)
  {
    IdToolsImpl idTools;
    map<uuid, rooset::MemberPrivilege> privileges;
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 1 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 2 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 3 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 4 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 5 };
    privileges[idTools.generateUniqueId()] = { true, false, true, true, 15 };

    const auto totalVoteWeight = VoteUtils::calcTotalVoteWeight(privileges);
    EXPECT_EQ(totalVoteWeight, 15);
  }
}

TEST(VoteUtils, isAdmissionQuorumPassed)
{
  rooset::Policy policy{
    "test policy",
    true,
    1,
    10
  };

  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 100, 10), true);
  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 101, 10), false);
  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 12, 7), true);
}



TEST(SchulzeBallot, getSchulzeRanking)
{
  IdToolsImpl idTools;
  uuid a = idTools.parse("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa");
  uuid b = idTools.parse("464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb");
  uuid c = idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc");
  uuid sq = idTools.generateNilId();

  SchulzeBallot p1({ {a} }, {}, { {b,c} });
  SchulzeBallot p3({ {a}, {b}, {c} }, {}, {});
  SchulzeBallot p4({ {c}, {b}, {a} }, {}, {});
  SchulzeBallot p5({ {c}, {b} }, {a}, {});

  vector<vector<uuid>> p1Ranking { {a}, {sq}, {b,c} };
  EXPECT_EQ(p1.getSchulzeRanking(), p1Ranking);

  vector<vector<uuid>> expectedP3Ranking { {a}, {b}, {c}, {sq} };
  EXPECT_EQ(p3.getSchulzeRanking(), expectedP3Ranking);

  vector<vector<uuid>> p4Ranking { {c}, {b}, {a}, {sq} };
  EXPECT_EQ(p4.getSchulzeRanking(), p4Ranking);

  vector<vector<uuid>> p5Ranking { {c}, {b}, {a,sq} };
  EXPECT_EQ(p5.getSchulzeRanking(), p5Ranking);
}


/*
 * All examples below are from
 * Schulze (draft, 23 December 2016), section 3
 * http://m-schulze.9mail.de/schulze1.pdf
 * 'a' in the text is replaced with statis quo
 */



TEST(VoteUtils_SchulzeExamples, example_1)
{
  IdToolsImpl idTools;
  uuid a = idTools.generateNilId(); 
  uuid b = idTools.parse("464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb");
  uuid c = idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc");
  uuid d = idTools.parse("464b1ebb-32c1-460c-8e9e-dddddddddddd");
  const vector<uuid> initiativeIds { a, b, c, d };
  
  SchulzeBallot p1({}, {}, { {c}, {d}, {b} });
  p1.setWeight(8);
  SchulzeBallot p2({ {b} }, {}, { {d} , {c} });
  p2.setWeight(2);
  SchulzeBallot p3({ {c}, {d}, {b} }, {}, {});
  p3.setWeight(4);
  SchulzeBallot p4({ {d}, {b}, }, {}, { {c} });
  p4.setWeight(4);

  // the rest are one line in the text, split up to make sure adding works
  SchulzeBallot p5({ {d}, {c}, {b}, }, {}, {});
  p5.setWeight(1);
  SchulzeBallot p6({ {d}, {c}, {b}, }, {}, {});
  p6.setWeight(1);
  SchulzeBallot p7({ {d}, {c}, {b}, }, {}, {});
  p7.setWeight(1);

  const vector<vector<unsigned long long>> expectedPairwiseMatrix {
    {  0,  8, 14, 10 },
    { 13,  0,  6,  2 },
    {  7, 15,  0, 12 },
    { 11, 19,  9,  0 },
  };

  const auto pairwiseMatrix = VoteUtils::calcPairwiseMatrix(
      { p1, p2, p3, p4, p5, p6, p7 }, initiativeIds);
  EXPECT_EQ(pairwiseMatrix, expectedPairwiseMatrix);

  vector<vector<vector<unsigned long long>>> strongestPathMatrix;
  vector<vector<int>> winningPairs;
  auto winners = VoteUtils::calcWinners(
    strongestPathMatrix, winningPairs, pairwiseMatrix);

  vector<vector<vector<unsigned long long>>> expectedstrongestPathMatrix {
    {    {  0,  0 },   { 14,  7 },   { 14,  7 },   { 12,  9 }    },
    {    { 13,  8 },   {  0,  0 },   { 13,  8 },   { 12,  9 }    },
    {    { 13,  8 },   { 15,  6 },   {  0,  0 },   { 12,  9 }    },
    {    { 13,  8 },   { 19,  2 },   { 13,  8 },   {  0,  0 }    },
  };
  const vector<vector<int>> expectedWinningPairs {
      {0,1}, {0,2}, {2,1}, {3,0}, {3,1}, {3,2} };
  const set<int> expectedWinners { 3 };

  EXPECT_EQ(strongestPathMatrix, expectedstrongestPathMatrix);
  EXPECT_THAT(winningPairs, testing::WhenSorted(testing::ElementsAreArray(expectedWinningPairs)));
  EXPECT_EQ(winners, expectedWinners);
}



TEST(VoteUtils_SchulzeExamples, example_2)
{
  IdToolsImpl idTools;
  uuid a = idTools.generateNilId(); 
  uuid b = idTools.parse("464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb");
  uuid c = idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc");
  uuid d = idTools.parse("464b1ebb-32c1-460c-8e9e-dddddddddddd");
  const vector<uuid> initiativeIds { a, b, c, d };
  
  SchulzeBallot p1({}, {}, { {b}, {c}, {d} });
  p1.setWeight(3);
  SchulzeBallot p2({ {c}, {b}, {d} }, {}, {});
  p2.setWeight(2);
  
  SchulzeBallot p3({ {d} }, {}, { {b}, {c} });
  p3.setWeight(1);
  SchulzeBallot p4({ {d} }, {}, { {b}, {c} });
  p4.setWeight(1);
  
  SchulzeBallot p5({ {d}, {b}, {c} }, {}, {});
  p5.setWeight(2);

  const vector<vector<unsigned long long>> expectedPairwiseMatrix {
    {  0,  5,  5,  3 },
    {  4,  0,  7,  5 },
    {  4,  2,  0,  5 },
    {  6,  4,  4,  0 },
  };

  const auto pairwiseMatrix = VoteUtils::calcPairwiseMatrix(
      { p1, p2, p3, p4, p5}, initiativeIds);
  EXPECT_EQ(pairwiseMatrix, expectedPairwiseMatrix);

  vector<vector<vector<unsigned long long>>> strongestPathMatrix;
  vector<vector<int>> winningPairs;
  auto winners = VoteUtils::calcWinners(
    strongestPathMatrix, winningPairs, pairwiseMatrix);

  vector<vector<vector<unsigned long long>>> expectedstrongestPathMatrix {
    {    {  0,  0 },  {  5,  4 },  {  5,  4 },  {  5,  4 }    },
    {    {  5,  4 },  {  0,  0 },  {  7,  2 },  {  5,  4 }    },
    {    {  5,  4 },  {  5,  4 },  {  0,  0 },  {  5,  4 }    },
    {    {  6,  3 },  {  5,  4 },  {  5,  4 },  {  0,  0 }    },
  };
  const vector<vector<int>> expectedWinningPairs {
      {1,2}, {3,0} };
  const set<int> expectedWinners { 1, 3 };

  EXPECT_EQ(strongestPathMatrix, expectedstrongestPathMatrix);
  EXPECT_THAT(winningPairs, testing::WhenSorted(testing::ElementsAreArray(expectedWinningPairs)));
  EXPECT_EQ(winners, expectedWinners);
}



class SchulzeExample5 : public ::testing::Test
{
protected:
  IdToolsImpl idTools;
  uuid a;
  uuid b;
  uuid c;
  uuid d;
  uuid e;
  uuid f;
  vector<uuid> initiativeIds;
  vector<unique_ptr<SchulzeBallot>> pBallots;
      
  virtual void SetUp()
  {
    a = idTools.generateNilId(); 
    b = idTools.parse("464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb");
    c = idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc");
    d = idTools.parse("464b1ebb-32c1-460c-8e9e-dddddddddddd");
    e = idTools.parse("464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee");
    f = idTools.parse("464b1ebb-32c1-460c-8e9e-ffffffffffff");
    initiativeIds = { a, b, c, d, e, f };

    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({}, {}, { {d}, {e}, {b}, {c}, {f} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({}, {}, { {d}, {e}, {b}, {c}, {f} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({}, {}, { {d}, {e}, {b}, {c}, {f} })));
    
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {b}, {f}, {e}, {c}, {d} }, {}, {})));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {b}, {f}, {e}, {c}, {d} }, {}, {})));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {b}, {f}, {e}, {c}, {d} }, {}, {})));
    
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {c} }, {}, { {b}, {f}, {d}, {e} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {c} }, {}, { {b}, {f}, {d}, {e} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {c} }, {}, { {b}, {f}, {d}, {e} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {c} }, {}, { {b}, {f}, {d}, {e} })));

    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {d}, {b}, {c}, {e}, {f} }, {}, {})));

    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {d}, {e}, {f} }, {}, { {b}, {c} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {d}, {e}, {f} }, {}, { {b}, {c} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {d}, {e}, {f} }, {}, { {b}, {c} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {d}, {e}, {f} }, {}, { {b}, {c} })));

    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {e}, {c}, {b}, {d}, {f} }, {}, {})));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {e}, {c}, {b}, {d}, {f} }, {}, {})));
    
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {f} }, {}, { {c}, {d}, {b}, {e} })));
    pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({ {f} }, {}, { {c}, {d}, {b}, {e} })));

    for (auto& b : pBallots) {
      b->setWeight(1);
    }
  }
};
  
    


TEST_F(SchulzeExample5, situation1)
{

  vector<SchulzeBallot> ballots;
  for (auto& b : pBallots) {
    ballots.push_back(*b);
  }
  ASSERT_EQ(ballots.at(0).getWeight(), 1);
  ASSERT_EQ(ballots.size(), 19);
  
  const vector<vector<unsigned long long>> expectedPairwiseMatrix {
    {  0, 13,  9,  9,  9,  7 },
    {  6,  0, 11,  9, 10, 13 },
    { 10,  8,  0, 11,  7, 10 },
    { 10, 10,  8,  0, 14, 10 },
    { 10,  9, 12,  5,  0, 10 },
    { 12,  6,  9,  9,  9,  0 }
  };

  auto pairwiseMatrix = VoteUtils::calcPairwiseMatrix(
      ballots, initiativeIds);
  EXPECT_EQ(pairwiseMatrix, expectedPairwiseMatrix);

  vector<vector<vector<unsigned long long>>> strongestPathMatrix;
  vector<vector<int>> winningPairs;
  auto winners = VoteUtils::calcWinners(
    strongestPathMatrix, winningPairs, pairwiseMatrix);

  vector<vector<vector<unsigned long long>>> expectedstrongestPathMatrix {
    {    {  0,  0 },  { 13,  6 },  { 11,  8 },  { 11,  8 },  { 11,  8 },  { 13,  6 }    },
    {    { 12,  7 },  {  0,  0 },  { 11,  8 },  { 11,  8 },  { 11,  8 },  { 13,  6 }    },
    {    { 10,  9 },  { 10,  9 },  {  0,  0 },  { 11,  8 },  { 11,  8 },  { 10,  9 }    },
    {    { 10,  9 },  { 10,  9 },  { 12,  7 },  {  0,  0 },  { 14,  5 },  { 10,  9 }    },
    {    { 10,  9 },  { 10,  9 },  { 12,  7 },  { 11,  8 },  {  0,  0 },  { 10,  9 }    },
    {    { 12,  7 },  { 12,  7 },  { 11,  8 },  { 11,  8 },  { 11,  8 },  {  0,  0 }    },
  }; 
  const vector<vector<int>> expectedWinningPairs {
      {0,1}, {0,2}, {0,3}, {0,4}, {0,5},
      {1,2}, {1,3}, {1,4}, {1,5},
      {3,2}, {3,4},
      {4,2},
      {5,2}, {5,3}, {5,4}
      };
  const set<int> expectedWinners { 0 };

  EXPECT_EQ(strongestPathMatrix, expectedstrongestPathMatrix);
  EXPECT_THAT(winningPairs, testing::WhenSorted(testing::ElementsAreArray(expectedWinningPairs)));
  EXPECT_EQ(winners, expectedWinners);
}



TEST_F(SchulzeExample5, situation2)
{
  pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({}, {}, { {e}, {f}, {c}, {b}, {d} })));
  pBallots.push_back(unique_ptr<SchulzeBallot>(new SchulzeBallot({}, {}, { {e}, {f}, {c}, {b}, {d} })));

  vector<SchulzeBallot> ballots;
  for (auto& b : pBallots) {
    b->setWeight(1);
    ballots.push_back(*b);
  }
  ASSERT_EQ(ballots.at(ballots.size() - 1).getWeight(), 1);

  const vector<vector<unsigned long long>> expectedPairwiseMatrix {
    {  0, 15, 11, 11, 11,  9 },
    {  6,  0, 11, 11, 10, 13 },
    { 10, 10,  0, 13,  7, 10 },
    { 10, 10,  8,  0, 14, 10 },
    { 10, 11, 14,  7,  0, 12 },
    { 12,  8, 11, 11,  9,  0 },
  };

  auto pairwiseMatrix = VoteUtils::calcPairwiseMatrix(
      ballots, initiativeIds);
  EXPECT_EQ(pairwiseMatrix, expectedPairwiseMatrix);

  vector<vector<vector<unsigned long long>>> expectedstrongestPathMatrix {
    {    {  0,  0 },  { 15,  6 },  { 11, 10 },  { 11, 10 },  { 11, 10 },  { 13,  8 }    },
    {    { 12,  9 },  {  0,  0 },  { 11, 10 },  { 11, 10 },  { 11, 10 },  { 13,  8 }    },
    {    { 12,  9 },  { 12,  9 },  {  0,  0 },  { 13,  8 },  { 13,  8 },  { 12,  9 }    },
    {    { 12,  9 },  { 12,  9 },  { 14,  7 },  {  0,  0 },  { 14,  7 },  { 12,  9 }    },
    {    { 12,  9 },  { 12,  9 },  { 14,  7 },  { 13,  8 },  {  0,  0 },  { 12,  9 }    },
    {    { 12,  9 },  { 12,  9 },  { 11, 10 },  { 11, 10 },  { 11, 10 },  {  0,  0 }    },
  }; 
  const vector<vector<int>> expectedWinningPairs {
      {0,1}, {0,5},
      {1,5},
      {2,0}, {2,1}, {2,5},
      {3,0}, {3,1}, {3,2}, {3,4}, {3,5},
      {4,0}, {4,1}, {4,2}, {4,5}   };
  const set<int> expectedWinners { 3 };

  vector<vector<vector<unsigned long long>>> strongestPathMatrix;
  vector<vector<int>> winningPairs;
  auto winners = VoteUtils::calcWinners(
    strongestPathMatrix, winningPairs, pairwiseMatrix);

  EXPECT_EQ(strongestPathMatrix, expectedstrongestPathMatrix);
  EXPECT_THAT(winningPairs, testing::WhenSorted(testing::ElementsAreArray(expectedWinningPairs)));
  EXPECT_EQ(winners, expectedWinners);
}



TEST(VoteUtils, comparePairwiseBattle)
{
  EXPECT_EQ(VoteUtils::comparePairwiseBattle({5,4}, {4,0}), 1);
  EXPECT_EQ(VoteUtils::comparePairwiseBattle({4,3}, {4,2}), -1);
  EXPECT_EQ(VoteUtils::comparePairwiseBattle({4,3}, {4,3}), 0);

}
