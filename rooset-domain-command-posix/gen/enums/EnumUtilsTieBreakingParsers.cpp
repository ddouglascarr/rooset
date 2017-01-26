
#include "enums/EnumUtils.h"
#include "enums/TieBreaking.h"
#include <stdexcept>

namespace rooset {
  TieBreaking EnumUtils::parseTieBreaking(const string& e)
  {
    if (e == "SIMPLE") return TieBreaking::SIMPLE;
if (e == "VARIANT1") return TieBreaking::VARIANT1;
if (e == "VARIANT2") return TieBreaking::VARIANT2;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeTieBreaking(TieBreaking e)
  {
    if (e == TieBreaking::SIMPLE) return "SIMPLE";
if (e == TieBreaking::VARIANT1) return "VARIANT1";
if (e == TieBreaking::VARIANT2) return "VARIANT2";
    throw std::invalid_argument("unknown enum");
  }

}  