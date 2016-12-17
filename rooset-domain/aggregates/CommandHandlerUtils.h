#pragma once
#include <stdexcept>
#include <string>
#include "exceptions/CommandEvaluationException.h"

using namespace std;
namespace rooset {

  class CommandHandlerUtils
  {
  public:
    template<typename Map, typename Item>
    static void assertMapContains(Map m, Item i, string message)
    {
      const bool isItemPresent = m.find(i) != m.end();
      if (!isItemPresent) {
        throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION, message);
      }
    }

    template<typename Map, typename Item>
    static void assertMapExcludes(Map m, Item i, string message)
    {
      const bool isItemPresent = m.find(i) != m.end();
      if (isItemPresent) {
        throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
            message);
      }
    }

  };
}