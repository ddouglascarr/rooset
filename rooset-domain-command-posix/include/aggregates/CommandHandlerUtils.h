#pragma once
#include <stdexcept>
#include <string>
#include <vector>
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

    template<typename Item>
    static void assertVectorContains(vector<Item> items, Item item, string message)
    {
      auto i = find(items.begin(), items.end(), item);
      if (i == items.end()) {
        throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
            message);
      }
    }

    template<typename Item>
    static void assertVectorExcludes(vector<Item> items, Item item, string message)
    {
      auto i = find(items.begin(), items.end(), item);
      if (i != items.end()) {
        throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
            message);
      }
    }

  };
}