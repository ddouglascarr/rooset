
#include "enums/EnumUtils.h"
#include "enums/DefeatStrength.h"
#include <stdexcept>

namespace rooset {
  DefeatStrength EnumUtils::parseDefeatStrength(const string& e)
  {
    if (e == "TUPLE") return DefeatStrength::TUPLE;
if (e == "SIMPLE") return DefeatStrength::SIMPLE;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeDefeatStrength(DefeatStrength e)
  {
    if (e == DefeatStrength::TUPLE) return "TUPLE";
if (e == DefeatStrength::SIMPLE) return "SIMPLE";
    throw std::invalid_argument("unknown enum");
  }

}  