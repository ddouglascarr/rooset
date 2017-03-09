
#include "enums/EnumUtils.h"
#include "enums/ContentType.h"
#include <stdexcept>

namespace rooset {
  ContentType EnumUtils::parseContentType(const string& e)
  {
    if (e == "STATEMENT") return ContentType::STATEMENT;
if (e == "GIT_PATCH") return ContentType::GIT_PATCH;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeContentType(ContentType e)
  {
    if (e == ContentType::STATEMENT) return "STATEMENT";
if (e == ContentType::GIT_PATCH) return "GIT_PATCH";
    throw std::invalid_argument("unknown enum");
  }

}  