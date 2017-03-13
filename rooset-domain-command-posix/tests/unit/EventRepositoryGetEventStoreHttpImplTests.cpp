#include "gmock/gmock.h"
#include <stdexcept>
#include "ratk/EventRepositoryGetEventStoreHttpImpl.h"
#include "ratk/IdToolsMockImpl.h"
#include "ratk/HttpToolsMockImpl.h"
#include "exceptions/CommandEvaluationException.h"
#include "events/IssueBallotUnsetEvent.h"
#include "rapidjson/document.h"

using namespace rooset;
using namespace std;
using ::testing::NiceMock;
using ::testing::Exactly;
using ::testing::Return;
using ::testing::ByMove;
using ::testing::_;



class EventRepositoryGetEventStoreHttpImplTests :
    public ::testing::Test
{
protected:
  shared_ptr<IdToolsMockImpl> mockIdTools = make_shared<
      NiceMock<IdToolsMockImpl>>();
  shared_ptr<HttpToolsMockImpl> mockHttpTools = make_shared<
      NiceMock<HttpToolsMockImpl>>();
  EventRepositoryGetEventStoreHttpImpl eventRepository;

public:
  EventRepositoryGetEventStoreHttpImplTests() :
      eventRepository(mockIdTools, mockHttpTools, "foohost", "1234")
  {}

  map<string, string> httpResponses;
  vector<rapidjson::Document> expectedEvents;

  void SetUp()
  {
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"10\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"11\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"12\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"13\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"14\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"15\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"16\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"17\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"18\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"19\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"20\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"21\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"22\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"23\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"24\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"25\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"26\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"27\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"28\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"29\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"30\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"31\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"32\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"33\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"34\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"35\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"36\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"37\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"38\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"39\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"40\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"41\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"42\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"43\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"44\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"45\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"46\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"47\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"48\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"49\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"50\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"51\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"52\" }"));
    expectedEvents.push_back(JsonUtils::parse("{ \"value\": \"53\" }"));

    httpResponses.emplace("self", R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-777777777777'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "updated": "2017-01-22T20:24:17.559572Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": true,
  "selfUrl": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "eTag": "43;248368668",
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0/forward/20",
      "relation": "last"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/23/backward/20",
      "relation": "next"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/44/forward/20",
      "relation": "previous"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/metadata",
      "relation": "metadata"
    }
  ],
  "entries": [
    {
      "title": "43@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
      "updated": "2017-01-22T20:24:17.559572Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_53",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "42@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
      "updated": "2017-01-22T20:24:17.522694Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_52",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "41@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
      "updated": "2017-01-22T20:24:17.486694Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_51",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "40@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
      "updated": "2017-01-22T20:24:17.448191Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_50",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "39@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
      "updated": "2017-01-22T20:24:17.410552Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_49",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "38@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
      "updated": "2017-01-22T20:24:17.373876Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_48",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "37@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
      "updated": "2017-01-22T20:24:17.335614Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_47",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "36@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
      "updated": "2017-01-22T20:24:17.297454Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_46",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "35@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
      "updated": "2017-01-22T20:24:17.261152Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_45",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "34@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
      "updated": "2017-01-22T20:24:17.225846Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_44",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "33@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
      "updated": "2017-01-22T20:24:17.191175Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_43",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "32@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
      "updated": "2017-01-22T20:24:17.152869Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_42",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "31@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
      "updated": "2017-01-22T20:24:17.116121Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_41",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "30@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
      "updated": "2017-01-22T20:24:17.079111Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_40",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "29@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
      "updated": "2017-01-22T20:24:17.041008Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_39",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "28@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
      "updated": "2017-01-22T20:24:17.003696Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_38",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "27@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
      "updated": "2017-01-22T20:24:16.969417Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_37",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "26@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
      "updated": "2017-01-22T20:24:16.932354Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_36",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "25@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
      "updated": "2017-01-22T20:24:16.897522Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_35",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
          "relation": "alternate"
        }
      ]
    },
    {
      "title": "24@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
      "updated": "2017-01-22T20:24:16.862763Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_34",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
          "relation": "alternate"
        }
      ]
    }
  ]
}
    )json");

  httpResponses.emplace("0forward20", R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-777777777777'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "updated": "2017-01-22T20:24:15.975978Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": false,
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/20/forward/20",
      "relation": "previous"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/metadata",
      "relation": "metadata"
    }
  ],
  "entries": [
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111129",
      "eventType": "EVENT_29",
      "eventNumber": 19,
      "data": "{ \"value\": \"29\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 19,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "19@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/19",
      "updated": "2017-01-22T20:24:16.679806Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_29",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/19",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/19",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111128",
      "eventType": "EVENT_28",
      "eventNumber": 18,
      "data": "{ \"value\": \"28\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 18,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "18@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/18",
      "updated": "2017-01-22T20:24:16.640111Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_28",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/18",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/18",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111127",
      "eventType": "EVENT_27",
      "eventNumber": 17,
      "data": "{ \"value\": \"27\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 17,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "17@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/17",
      "updated": "2017-01-22T20:24:16.604625Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_27",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/17",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/17",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111126",
      "eventType": "EVENT_26",
      "eventNumber": 16,
      "data": "{ \"value\": \"26\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 16,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "16@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/16",
      "updated": "2017-01-22T20:24:16.567261Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_26",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/16",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/16",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111125",
      "eventType": "EVENT_25",
      "eventNumber": 15,
      "data": "{ \"value\": \"25\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 15,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "15@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/15",
      "updated": "2017-01-22T20:24:16.526689Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_25",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/15",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/15",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111124",
      "eventType": "EVENT_24",
      "eventNumber": 14,
      "data": "{ \"value\": \"24\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 14,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "14@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/14",
      "updated": "2017-01-22T20:24:16.486565Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_24",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/14",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/14",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111123",
      "eventType": "EVENT_23",
      "eventNumber": 13,
      "data": "{ \"value\": \"23\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 13,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "13@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/13",
      "updated": "2017-01-22T20:24:16.451301Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_23",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/13",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/13",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111122",
      "eventType": "EVENT_22",
      "eventNumber": 12,
      "data": "{ \"value\": \"22\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 12,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "12@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/12",
      "updated": "2017-01-22T20:24:16.416267Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_22",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/12",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/12",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111121",
      "eventType": "EVENT_21",
      "eventNumber": 11,
      "data": "{ \"value\": \"21\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 11,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "11@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/11",
      "updated": "2017-01-22T20:24:16.381168Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_21",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/11",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/11",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111120",
      "eventType": "EVENT_20",
      "eventNumber": 10,
      "data": "{ \"value\": \"20\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 10,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "10@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/10",
      "updated": "2017-01-22T20:24:16.346393Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_20",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/10",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/10",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111119",
      "eventType": "EVENT_19",
      "eventNumber": 9,
      "data": "{ \"value\": \"19\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 9,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "9@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/9",
      "updated": "2017-01-22T20:24:16.310835Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_19",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/9",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/9",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111118",
      "eventType": "EVENT_18",
      "eventNumber": 8,
      "data": "{ \"value\": \"18\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 8,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "8@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/8",
      "updated": "2017-01-22T20:24:16.276336Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_18",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/8",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/8",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111117",
      "eventType": "EVENT_17",
      "eventNumber": 7,
      "data": "{ \"value\": \"17\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 7,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "7@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/7",
      "updated": "2017-01-22T20:24:16.241453Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_17",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/7",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/7",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111116",
      "eventType": "EVENT_16",
      "eventNumber": 6,
      "data": "{ \"value\": \"16\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 6,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "6@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/6",
      "updated": "2017-01-22T20:24:16.20632Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_16",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/6",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/6",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111115",
      "eventType": "EVENT_15",
      "eventNumber": 5,
      "data": "{ \"value\": \"15\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 5,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "5@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/5",
      "updated": "2017-01-22T20:24:16.163976Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_15",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/5",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/5",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111114",
      "eventType": "EVENT_14",
      "eventNumber": 4,
      "data": "{ \"value\": \"14\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 4,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "4@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/4",
      "updated": "2017-01-22T20:24:16.12866Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_14",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/4",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/4",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111113",
      "eventType": "EVENT_13",
      "eventNumber": 3,
      "data": "{ \"value\": \"13\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 3,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "3@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/3",
      "updated": "2017-01-22T20:24:16.091286Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_13",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/3",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/3",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111112",
      "eventType": "EVENT_12",
      "eventNumber": 2,
      "data": "{ \"value\": \"12\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 2,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "2@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/2",
      "updated": "2017-01-22T20:24:16.054277Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_12",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/2",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/2",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111111",
      "eventType": "EVENT_11",
      "eventNumber": 1,
      "data": "{ \"value\": \"11\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 1,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "1@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/1",
      "updated": "2017-01-22T20:24:16.016532Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_11",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/1",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/1",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111110",
      "eventType": "EVENT_10",
      "eventNumber": 0,
      "data": "{ \"value\": \"10\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 0,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "0@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0",
      "updated": "2017-01-22T20:24:15.975978Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_10",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0",
          "relation": "alternate"
        }
      ]
    }
  ]
}

  )json");

  httpResponses.emplace("20forward20", u8R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-777777777777'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "updated": "2017-01-22T20:24:16.717662Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": false,
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0/forward/20",
      "relation": "last"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/19/backward/20",
      "relation": "next"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40/forward/20",
      "relation": "previous"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/metadata",
      "relation": "metadata"
    }
  ],
  "entries": [
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111149",
      "eventType": "EVENT_49",
      "eventNumber": 39,
      "data": "{ \"value\": \"49\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 39,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "39@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
      "updated": "2017-01-22T20:24:17.410552Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_49",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111148",
      "eventType": "EVENT_48",
      "eventNumber": 38,
      "data": "{ \"value\": \"48\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 38,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "38@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
      "updated": "2017-01-22T20:24:17.373876Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_48",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/38",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111147",
      "eventType": "EVENT_47",
      "eventNumber": 37,
      "data": "{ \"value\": \"47\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 37,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "37@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
      "updated": "2017-01-22T20:24:17.335614Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_47",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/37",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111146",
      "eventType": "EVENT_46",
      "eventNumber": 36,
      "data": "{ \"value\": \"46\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 36,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "36@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
      "updated": "2017-01-22T20:24:17.297454Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_46",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/36",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111145",
      "eventType": "EVENT_45",
      "eventNumber": 35,
      "data": "{ \"value\": \"45\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 35,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "35@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
      "updated": "2017-01-22T20:24:17.261152Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_45",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/35",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111144",
      "eventType": "EVENT_44",
      "eventNumber": 34,
      "data": "{ \"value\": \"44\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 34,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "34@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
      "updated": "2017-01-22T20:24:17.225846Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_44",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/34",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111143",
      "eventType": "EVENT_43",
      "eventNumber": 33,
      "data": "{ \"value\": \"43\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 33,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "33@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
      "updated": "2017-01-22T20:24:17.191175Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_43",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/33",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111142",
      "eventType": "EVENT_42",
      "eventNumber": 32,
      "data": "{ \"value\": \"42\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 32,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "32@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
      "updated": "2017-01-22T20:24:17.152869Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_42",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/32",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111141",
      "eventType": "EVENT_41",
      "eventNumber": 31,
      "data": "{ \"value\": \"41\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 31,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "31@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
      "updated": "2017-01-22T20:24:17.116121Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_41",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/31",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111140",
      "eventType": "EVENT_40",
      "eventNumber": 30,
      "data": "{ \"value\": \"40\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 30,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "30@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
      "updated": "2017-01-22T20:24:17.079111Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_40",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/30",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111139",
      "eventType": "EVENT_39",
      "eventNumber": 29,
      "data": "{ \"value\": \"39\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 29,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "29@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
      "updated": "2017-01-22T20:24:17.041008Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_39",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/29",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111138",
      "eventType": "EVENT_38",
      "eventNumber": 28,
      "data": "{ \"value\": \"38\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 28,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "28@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
      "updated": "2017-01-22T20:24:17.003696Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_38",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/28",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111137",
      "eventType": "EVENT_37",
      "eventNumber": 27,
      "data": "{ \"value\": \"37\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 27,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "27@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
      "updated": "2017-01-22T20:24:16.969417Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_37",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/27",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111136",
      "eventType": "EVENT_36",
      "eventNumber": 26,
      "data": "{ \"value\": \"36\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 26,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "26@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
      "updated": "2017-01-22T20:24:16.932354Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_36",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/26",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111135",
      "eventType": "EVENT_35",
      "eventNumber": 25,
      "data": "{ \"value\": \"35\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 25,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "25@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
      "updated": "2017-01-22T20:24:16.897522Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_35",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/25",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111134",
      "eventType": "EVENT_34",
      "eventNumber": 24,
      "data": "{ \"value\": \"34\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 24,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "24@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
      "updated": "2017-01-22T20:24:16.862763Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_34",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/24",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111133",
      "eventType": "EVENT_33",
      "eventNumber": 23,
      "data": "{ \"value\": \"33\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 23,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "23@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/23",
      "updated": "2017-01-22T20:24:16.826367Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_33",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/23",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/23",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111132",
      "eventType": "EVENT_32",
      "eventNumber": 22,
      "data": "{ \"value\": \"32\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 22,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "22@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/22",
      "updated": "2017-01-22T20:24:16.790004Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_32",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/22",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/22",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111131",
      "eventType": "EVENT_31",
      "eventNumber": 21,
      "data": "{ \"value\": \"31\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 21,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "21@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/21",
      "updated": "2017-01-22T20:24:16.753004Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_31",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/21",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/21",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111130",
      "eventType": "EVENT_30",
      "eventNumber": 20,
      "data": "{ \"value\": \"30\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 20,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "20@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/20",
      "updated": "2017-01-22T20:24:16.717662Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_30",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/20",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/20",
          "relation": "alternate"
        }
      ]
    }
  ]
}
  )json");

  httpResponses.emplace("40forward20", u8R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-777777777777'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "updated": "2017-01-22T20:24:17.448191Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": true,
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0/forward/20",
      "relation": "last"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/39/backward/20",
      "relation": "next"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/44/forward/20",
      "relation": "previous"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/metadata",
      "relation": "metadata"
    }
  ],
  "entries": [
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111153",
      "eventType": "EVENT_53",
      "eventNumber": 43,
      "data": "{ \"value\": \"53\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 43,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "43@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
      "updated": "2017-01-22T20:24:17.559572Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_53",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111152",
      "eventType": "EVENT_52",
      "eventNumber": 42,
      "data": "{ \"value\": \"52\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 42,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "42@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
      "updated": "2017-01-22T20:24:17.522694Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_52",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/42",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111151",
      "eventType": "EVENT_51",
      "eventNumber": 41,
      "data": "{ \"value\": \"51\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 41,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "41@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
      "updated": "2017-01-22T20:24:17.486694Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_51",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/41",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460c-8e9e-111111111150",
      "eventType": "EVENT_50",
      "eventNumber": 40,
      "data": "{ \"value\": \"50\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 40,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-777777777777",
      "title": "40@464b1ebb-32c1-460c-8e9e-777777777777",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
      "updated": "2017-01-22T20:24:17.448191Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_50",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40",
          "relation": "alternate"
        }
      ]
    }
  ]
}
  )json");

  httpResponses.emplace("44forward20", u8R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-777777777777'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
  "updated": "0001-01-01T00:00:00Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-777777777777",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": true,
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0/forward/20",
      "relation": "last"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/43/backward/20",
      "relation": "next"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/metadata",
      "relation": "metadata"
    }
  ],
  "entries": []
}
  )json");

  httpResponses.emplace("allEntriesOnFirstPage", u8R"json(
{
  "title": "Event stream '464b1ebb-32c1-460c-8e9e-888888888888'",
  "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888",
  "updated": "2017-01-23T00:11:54.042654Z",
  "streamId": "464b1ebb-32c1-460c-8e9e-888888888888",
  "author": {
    "name": "EventStore"
  },
  "headOfStream": true,
  "selfUrl": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888",
  "eTag": "3;248368668",
  "links": [
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888",
      "relation": "self"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/head/backward/20",
      "relation": "first"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/4/forward/20",
      "relation": "previous"
    },
    {
      "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/metadata",
      "relation": "metadata"
    }
  ],
  "entries": [
    {
      "eventId": "464b1ebb-32c1-460d-8e9e-211111111113",
      "eventType": "EVENT_13",
      "eventNumber": 3,
      "data": "{ \"value\": \"13\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 3,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "title": "3@464b1ebb-32c1-460c-8e9e-888888888888",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/3",
      "updated": "2017-01-23T00:11:54.042654Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_13",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/3",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/3",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460d-8e9e-211111111112",
      "eventType": "EVENT_12",
      "eventNumber": 2,
      "data": "{ \"value\": \"12\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 2,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "title": "2@464b1ebb-32c1-460c-8e9e-888888888888",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/2",
      "updated": "2017-01-23T00:11:54.005334Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_12",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/2",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/2",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460d-8e9e-211111111111",
      "eventType": "EVENT_11",
      "eventNumber": 1,
      "data": "{ \"value\": \"11\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 1,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "title": "1@464b1ebb-32c1-460c-8e9e-888888888888",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/1",
      "updated": "2017-01-23T00:11:53.961689Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_11",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/1",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/1",
          "relation": "alternate"
        }
      ]
    },
    {
      "eventId": "464b1ebb-32c1-460d-8e9e-211111111110",
      "eventType": "EVENT_10",
      "eventNumber": 0,
      "data": "{ \"value\": \"10\" }",
      "streamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "isJson": true,
      "isMetaData": false,
      "isLinkMetaData": false,
      "positionEventNumber": 0,
      "positionStreamId": "464b1ebb-32c1-460c-8e9e-888888888888",
      "title": "0@464b1ebb-32c1-460c-8e9e-888888888888",
      "id": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/0",
      "updated": "2017-01-23T00:11:53.915602Z",
      "author": {
        "name": "EventStore"
      },
      "summary": "EVENT_10",
      "links": [
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/0",
          "relation": "edit"
        },
        {
          "uri": "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/0",
          "relation": "alternate"
        }
      ]
    }
  ]
}
  )json");
  }
};



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    save_shouldSaveEvent)
{
  const auto aggregateId = mockIdTools->parse("464b1ebb-32c1-460c-8e9e-444444444444");
  const auto eventId = mockIdTools->parse("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa");
  vector<string> expectedHeaders {
    "Accept: application/json",
    "Content-Type: application/json",
    "ES-EventType: ISSUE_BALLOT_UNSET_EVENT",
    "ES-EventId: 464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
  };
  const IssueBallotUnsetEvent evt(
      aggregateId,
      mockIdTools->parse("464b1ebb-32c1-460c-8e9e-555555555555"));

  EXPECT_CALL(*mockIdTools, generateUniqueId())
      .Times(Exactly(1))
      .WillOnce(Return(eventId));

  EXPECT_CALL(*mockHttpTools, post(
      "foohost", "1234",
      expectedHeaders,
      "/streams/aggregate-464b1ebb-32c1-460c-8e9e-444444444444",
      JsonUtils::serialize(*evt.serialize())))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(201, "")));

  auto returnedUuid = eventRepository.save(evt);
  EXPECT_EQ(returnedUuid, eventId);
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    save_shouldThrowOnNotSuccess)
{
  const auto aggregateId = mockIdTools->parse("464b1ebb-32c1-460c-8e9e-444444444444");
  const auto eventId = mockIdTools->parse("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa");
  const IssueBallotUnsetEvent evt(
      aggregateId,
      mockIdTools->parse("464b1ebb-32c1-460c-8e9e-555555555555"));

  ON_CALL(*mockIdTools, generateUniqueId())
      .WillByDefault(Return(eventId));

  ON_CALL(*mockHttpTools, post(_, _, _, _, _))
      .WillByDefault(Return(HttpResponse(400, "")));

  try {
    eventRepository.save(evt);
    throw runtime_error("should have thrown");
  } catch (const CommandEvaluationException& e) {
    EXPECT_EQ(e.getExceptionCode(), ExceptionCode::GENERAL_PROJECT_EXCEPTION);
  }
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    assertAggregateDoesNotExist_throwsOnDoesExist)
{
  vector<string> expectedHeaders { "Accept: application/vnd.eventstore.atom+json" };
  EXPECT_CALL(*mockHttpTools, get(
      "foohost", "1234",
      expectedHeaders,
      "/streams/aggregate-464b1ebb-32c1-460c-8e9e-111111111111"))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(200, u8R"json({ "foo": "bar" })json")));

  try {
    eventRepository.assertAggregateDoesNotExist(
        mockIdTools->parse("464b1ebb-32c1-460c-8e9e-111111111111"));
    throw runtime_error("should have thrown");
  } catch (const CommandEvaluationException& e) {
    EXPECT_EQ(e.getExceptionCode(), ExceptionCode::CONFLICT_EXCEPTION);
  }
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    assertAggregateDoesNotExist_doesNothinOnExist)
{
  vector<string> expectedHeaders { "Accept: application/vnd.eventstore.atom+json" };
  EXPECT_CALL(*mockHttpTools, get(
      "foohost", "1234",
      expectedHeaders,
      "/streams/aggregate-464b1ebb-32c1-460c-8e9e-111111111111"))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(404, "")));

  eventRepository.assertAggregateDoesNotExist(
      mockIdTools->parse("464b1ebb-32c1-460c-8e9e-111111111111"));
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    loadAggregateEvents_returnsEvents)
{
  const auto aggregateId = mockIdTools->parse("464b1ebb-32c1-460c-8e9e-777777777777");
  vector<string> expectedHeaders { "Accept: application/vnd.eventstore.atom+json" };
  EXPECT_CALL(*mockHttpTools, get(
    "foohost", "1234",
    expectedHeaders,
    "/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777"))
    .Times(Exactly(1))
    .WillOnce(Return(HttpResponse(200, httpResponses.at("self"))));

  EXPECT_CALL(*mockHttpTools, get(
    expectedHeaders,
    "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/0/forward/20?embed=body"))
    .Times(Exactly(1))
    .WillOnce(Return(HttpResponse(200, httpResponses.at("0forward20"))));

  EXPECT_CALL(*mockHttpTools, get(
    expectedHeaders,
    "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/20/forward/20?embed=body"))
    .Times(Exactly(1))
    .WillOnce(Return(HttpResponse(200, httpResponses.at("20forward20"))));

  EXPECT_CALL(*mockHttpTools, get(
    expectedHeaders,
    "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/40/forward/20?embed=body"))
    .Times(Exactly(1))
    .WillOnce(Return(HttpResponse(200, httpResponses.at("40forward20"))));

  EXPECT_CALL(*mockHttpTools, get(
    expectedHeaders,
    "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-777777777777/44/forward/20?embed=body"))
    .Times(Exactly(1))
    .WillOnce(Return(HttpResponse(200, httpResponses.at("44forward20"))));


  vector<rapidjson::Document> events = eventRepository.loadAggregateEvents(aggregateId);
  for (int i = 0; i < events.size(); ++i) {
    EXPECT_EQ(events[i], expectedEvents[i]);
  }
  EXPECT_EQ(events.size(), expectedEvents.size());
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    loadAggregateEvents_allEventsOnOnePage)
{
  auto aggregateId = IdUtils::parse("464b1ebb-32c1-460c-8e9e-888888888888");
  vector <rapidjson::Document> expected;
  expected.push_back(JsonUtils::parse("{ \"value\": \"10\" }"));
  expected.push_back(JsonUtils::parse("{ \"value\": \"11\" }"));
  expected.push_back(JsonUtils::parse("{ \"value\": \"12\" }"));
  expected.push_back(JsonUtils::parse("{ \"value\": \"13\" }"));
  vector<string> expectedHeaders { "Accept: application/vnd.eventstore.atom+json" };

  EXPECT_CALL(*mockHttpTools, get(
      "foohost", "1234",
      expectedHeaders,
      "/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888"))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(200, httpResponses.at("allEntriesOnFirstPage"))));

  EXPECT_CALL(*mockHttpTools, get(
      expectedHeaders,
      "http://foohost:1234/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888?embed=body"))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(200, httpResponses.at("allEntriesOnFirstPage"))));

  // 44forward20 is not what its asking for, but is functionaly the same
  EXPECT_CALL(*mockHttpTools, get(
      expectedHeaders,
      "http://localhost:2113/streams/aggregate-464b1ebb-32c1-460c-8e9e-888888888888/4/forward/20?embed=body"))
      .Times(Exactly(1))
      .WillOnce(Return(HttpResponse(200, httpResponses.at("44forward20"))));

  vector<rapidjson::Document> events = eventRepository.loadAggregateEvents(aggregateId);
  for (int i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(events[i], expectedEvents[i]);
  }
  EXPECT_EQ(expected.size(), events.size());
}



TEST_F(EventRepositoryGetEventStoreHttpImplTests,
    loadAggregateEvents_throwsOnDoesNotExist)
{
  ON_CALL(*mockHttpTools, get(_, _, _, _))
      .WillByDefault(Return(HttpResponse(404, "")));
  try {
    eventRepository.loadAggregateEvents(mockIdTools->parse(
        "464b1ebb-32c1-460c-8e9e-111111111111"));
    throw std::runtime_error("should have thrown");
  } catch (const CommandEvaluationException& e) {
    EXPECT_EQ(e.getExceptionCode(), ExceptionCode::ITEM_NOT_FOUND_EXCEPTION);
  }
}
