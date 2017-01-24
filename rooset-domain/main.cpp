#include <memory>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include "rapidjson/document.h"
#include "framework/ProjectException.h"
#include "framework/JsonUtils.h"
#include "framework/EventRepositoryGetEventStoreHttpImpl.h"
#include "framework/CommandServiceImpl.h"
#include "framework/IdToolsImpl.h"
#include "framework/IdUtils.h"
#include "framework/HttpToolsCurlImpl.h"
#include "aggregates/CommandHandler.h"
#include "exceptions/SystemException.h"

using namespace std;
using namespace rooset;


int main()
{
  try {

    string sJson;
    string buffer;
    while(getline(cin, buffer)) { sJson += buffer; }
    const char* eventStoreHost = std::getenv("ROOSET_EVENT_STORE_HOST");
    const char* eventStorePort = std::getenv("ROOSET_EVENT_STORE_PORT");

    if (eventStoreHost == nullptr) throw invalid_argument(
        "$ROOSET_EVENT_STORE_HOST must be set");
    if (eventStorePort == nullptr) throw invalid_argument(
        "$ROOSET_EVENT_STORE_PORT must be set");

    shared_ptr<EventRepository> eventRepository = make_shared<
        EventRepositoryGetEventStoreHttpImpl> (
        make_shared<IdToolsImpl>(),
        make_shared<HttpToolsCurlImpl>(),
        eventStoreHost, eventStorePort);

    CommandHandler commandHandler(eventRepository);
    CommandServiceImpl<CommandHandler> commandService(eventRepository, commandHandler);

    uuid eventId = commandService.evaluate(sJson);
    cout << "\n{ \"eventId\": \"" << IdUtils::serialize(eventId) <<
        "\" }\n" << "\n";

  } catch (const ProjectException<rapidjson::Document>& e) {
    cout << "\n" << JsonUtils::serialize(*e.serialize()) << "\n";
    return 1;
  } catch (const exception& e) {
    
    SystemException systemException(
        ExceptionCode::GENERAL_PROJECT_EXCEPTION,
        e.what());
    cout << "\n" << JsonUtils::serialize(*systemException.serialize()) << "\n";
    return 1;
  }
}
