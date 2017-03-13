#include "EventRepositoryGetEventStoreHttpImpl.h"
#include "exceptions/CommandEvaluationException.h"

using namespace rooset;


rooset::uuid rooset::EventRepositoryGetEventStoreHttpImpl::save(
    const ProjectEvent<rapidjson::Document>& e)
{
  const uuid eventId = idTools->generateUniqueId();
  const string eventType = e.getMessageType();
  const uuid aggregateId = e.getAggregateId();
  auto resp = httpTools->post(
      host, port,
      getPostHeaders(eventId, eventType),
      "/streams/aggregate-" + idTools->serialize(aggregateId),
      JsonUtils::serialize(*e.serialize()));

  if (resp.status != 201) throw CommandEvaluationException(
      ExceptionCode::GENERAL_PROJECT_EXCEPTION,
      "The request failed with status: " + std::to_string(resp.status));

  return eventId;
}



/*
 * Described at http://docs.geteventstore.com/http-api/4.0.0/reading-streams
 */
vector<rapidjson::Document> rooset::EventRepositoryGetEventStoreHttpImpl
    ::loadAggregateEvents(const uuid& aggregateId)
{
  auto resp = httpTools->get(
      host, port,
      getGetHeaders(),
      "/streams/aggregate-" + idTools->serialize(aggregateId));

  if (resp.status != 200) throw CommandEvaluationException(
      ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
      "Failed to access aggregate, curl failed with code: " + std::to_string(resp.status));
  auto body = JsonUtils::parse(resp.body);
  vector<rapidjson::Document> events;
  string lastUri = "";
  for ( auto& link : body["links"].GetArray()) {
    if ("last" == string(link["relation"].GetString())) {
      lastUri = link["uri"].GetString();
    }
  }
  if (lastUri == "") {
    getEventRange(events, "http://" + host + ":" + port + "/streams/" +
        "aggregate-" + idTools->serialize(aggregateId));
  } else {
    getEventRange(events, lastUri);
  }
  return events;
}



void rooset::EventRepositoryGetEventStoreHttpImpl::assertAggregateDoesNotExist(
    const uuid& aggregateId)
{
  HttpResponse resp = httpTools->get(
      host, port, getGetHeaders(),
      "/streams/aggregate-" + idTools->serialize(aggregateId));
  if (resp.status != 404) throw CommandEvaluationException(
      ExceptionCode::CONFLICT_EXCEPTION,
      "The aggregate (" + idTools->serialize(aggregateId) + ") should not exist, but it does");
}



const vector<string> rooset::EventRepositoryGetEventStoreHttpImpl::getGetHeaders()
{
  return { "Accept: application/vnd.eventstore.atom+json" };
}



const vector<string> rooset::EventRepositoryGetEventStoreHttpImpl::getPostHeaders(
    const uuid& eventId,
    const string& eventType)
{
  return {
    "Accept: application/json",
    "Content-Type: application/json",
    "ES-EventType: " + eventType,
    "ES-EventId: " + idTools->serialize(eventId),
  };
};



void rooset::EventRepositoryGetEventStoreHttpImpl::getEventRange(
    vector<rapidjson::Document>& events,
    const string& uri)
{
  HttpResponse resp = httpTools->get(getGetHeaders(), uri + "?embed=body");
  if (resp.status != 200) throw runtime_error(
       "Failed to fetch GetEventStore events at \n" + uri + "\n");
  auto body = JsonUtils::parse(resp.body);

  string previousUri = "";
  for (auto& link : body["links"].GetArray()) {
    if("previous" == string(link["relation"].GetString())) {
      previousUri = link["uri"].GetString();
    }
  }

  JsonUtils::forEachReverse(
      body["entries"],
      [&](const rapidjson::Value& entry) {
        events.push_back(JsonUtils::parse(entry["data"].GetString()));
      });

  if (previousUri != "") getEventRange(events, previousUri);
}
