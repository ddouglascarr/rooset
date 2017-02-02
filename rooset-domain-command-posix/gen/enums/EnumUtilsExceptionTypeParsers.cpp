
#include "enums/EnumUtils.h"
#include "enums/ExceptionType.h"
#include <stdexcept>

namespace rooset {
  ExceptionType EnumUtils::parseExceptionType(const string& e)
  {
    if (e == "COMMAND_EVALUATION_EXCEPTION") return ExceptionType::COMMAND_EVALUATION_EXCEPTION;
if (e == "SYSTEM_EXCEPTION") return ExceptionType::SYSTEM_EXCEPTION;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeExceptionType(ExceptionType e)
  {
    if (e == ExceptionType::COMMAND_EVALUATION_EXCEPTION) return "COMMAND_EVALUATION_EXCEPTION";
if (e == ExceptionType::SYSTEM_EXCEPTION) return "SYSTEM_EXCEPTION";
    throw std::invalid_argument("unknown enum");
  }

}  