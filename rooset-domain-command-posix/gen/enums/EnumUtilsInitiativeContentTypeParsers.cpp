
#include "enums/EnumUtils.h"
#include "enums/InitiativeContentType.h"
#include <stdexcept>

namespace rooset {
  InitiativeContentType EnumUtils::parseInitiativeContentType(const string& e)
  {
    if (e == "STATEMENT") return InitiativeContentType::STATEMENT;
if (e == "GIT_PATCH") return InitiativeContentType::GIT_PATCH;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeInitiativeContentType(InitiativeContentType e)
  {
    if (e == InitiativeContentType::STATEMENT) return "STATEMENT";
if (e == InitiativeContentType::GIT_PATCH) return "GIT_PATCH";
    throw std::invalid_argument("unknown enum");
  }

}  