#include <memory>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "rapidjson/document.h"
#include "framework/ProjectException.h"
#include "framework/JsonUtils.h"
#include "framework/EventRepositoryGetEventStoreHttpImpl.h"
#include "framework/CommandServiceImpl.h"
#include "framework/IdToolsImpl.h"
#include "framework/IdUtils.h"
#include "framework/HttpToolsCurlImpl.h"
#include "aggregates/CommandHandler.h"

using namespace std;
using namespace rooset;


int main()
{
  try {

    string sJson;
    string buffer;
    while(getline(cin, buffer)) { sJson += buffer; }

    shared_ptr<EventRepository> eventRepository = make_shared<
        EventRepositoryGetEventStoreHttpImpl> (
        make_shared<IdToolsImpl>(),
        make_shared<HttpToolsCurlImpl>(),
        "localhost", "2113");

    CommandHandler commandHandler(eventRepository);
    CommandServiceImpl<CommandHandler> commandService(eventRepository, commandHandler);

    uuid eventId = commandService.evaluate(sJson);
    cout << "\n{ \"eventId\": \"" << IdUtils::serialize(eventId) <<
        "\" }\n" << "\n";

  } catch (const ProjectException<rapidjson::Document>& e) {
    cout << "\n" << JsonUtils::serialize(*e.serialize()) << "\n";
    return 1;
  } catch (const exception& e) {
    cout << "\n" << e.what() << "\n";
    return 1;
  }
}
