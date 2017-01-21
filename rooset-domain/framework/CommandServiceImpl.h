#include "CommandService.h"

#include <stdexcept>
#include <memory>
#include "rapidjson/document.h"
#include "EventRepository.h"
#include "JsonUtils.h"
#include "commands/CommandUtils.h"
#include "exceptions/CommandEvaluationException.h"


using namespace std;
namespace rooset {

  template<typename CommandHandler>
  class CommandServiceImpl : public CommandService
  {
  protected:
    shared_ptr<EventRepository> eventRepository;
    CommandHandler& commandHandler;

  public:
    CommandServiceImpl(
        shared_ptr<EventRepository> eventRepository,
        CommandHandler& commandHandler) :
        eventRepository(move(eventRepository)),
        commandHandler(commandHandler)
    {};

    inline uuid evaluate(const string& sJson) override
    {
      try {
        auto json = JsonUtils::parse(sJson);
        auto evt = CommandUtils::evaluate<CommandHandler>(commandHandler, *json);
        if (evt == nullptr) throw CommandEvaluationException(
            ExceptionCode::GENERAL_PROJECT_EXCEPTION,
            "command handler returned nullptr.\
                - check that the evaluate command is implemented");
        return eventRepository->save(*evt);
      } catch (const invalid_argument& e) {
        throw CommandEvaluationException(
            ExceptionCode::INVALID_INPUT_EXCEPTION,
            e.what());
      }
    };
  };

}
