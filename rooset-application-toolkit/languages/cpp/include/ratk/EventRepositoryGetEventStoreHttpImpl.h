#pragma once
#include <memory>
#include "EventRepository.h"
#include "IdTools.h"
#include "HttpTools.h"

using namespace std;
namespace rooset {



class EventRepositoryGetEventStoreHttpImpl :
    public EventRepository
{
protected:
  shared_ptr<IdTools> idTools;
  shared_ptr<HttpTools> httpTools;
  const string host;
  const string port;

public:
  inline EventRepositoryGetEventStoreHttpImpl(
      const shared_ptr<IdTools>& idTools,
      const shared_ptr<HttpTools>& httpTools,
      const string& eventStoreHttpHost,
      const string& eventStoreHttpPort) :
      idTools(idTools),
      httpTools(httpTools),
      host(eventStoreHttpHost),
      port(eventStoreHttpPort)
  {}

  uuid save(const ProjectEvent<rapidjson::Document>& e) override;

  vector<rapidjson::Document> loadAggregateEvents(
      const uuid& aggregateId) override;

  void assertAggregateDoesNotExist(const uuid& aggregateId) override;

private:
  const vector<string> getGetHeaders();
  const vector<string> getPostHeaders(
      const uuid& eventId,
      const string& eventType);

  void getEventRange(
      vector<rapidjson::Document>& events,
      const string& url);
};



}
