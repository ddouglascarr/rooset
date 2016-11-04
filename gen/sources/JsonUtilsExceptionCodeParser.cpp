
#include "framework/JsonUtils.h"
#include "enums/ExceptionCode.h"
#include <stdexcept>

namespace rooset {
  ExceptionCode JsonUtils::parseExceptionCode(const string& e)
  {
    if (e == "CONFLICT_EXCEPTION") return ExceptionCode::CONFLICT_EXCEPTION;
if (e == "GENERAL_PROJECT_EXCEPTION") return ExceptionCode::GENERAL_PROJECT_EXCEPTION;
if (e == "ITEM_NOT_FOUND_EXCEPTION") return ExceptionCode::ITEM_NOT_FOUND_EXCEPTION;
if (e == "UNAUTHORIZED_EXCEPTION") return ExceptionCode::UNAUTHORIZED_EXCEPTION;
if (e == "UNPRIVILEGED_EXCEPTION") return ExceptionCode::UNPRIVILEGED_EXCEPTION;
if (e == "INVALID_INPUT_EXCEPTION") return ExceptionCode::INVALID_INPUT_EXCEPTION;
    throw std::invalid_argument("unknown enum");
  }

  string JsonUtils::serializeExceptionCode(ExceptionCode e)
  {
    if (e == ExceptionCode::CONFLICT_EXCEPTION) return "CONFLICT_EXCEPTION";
if (e == ExceptionCode::GENERAL_PROJECT_EXCEPTION) return "GENERAL_PROJECT_EXCEPTION";
if (e == ExceptionCode::ITEM_NOT_FOUND_EXCEPTION) return "ITEM_NOT_FOUND_EXCEPTION";
if (e == ExceptionCode::UNAUTHORIZED_EXCEPTION) return "UNAUTHORIZED_EXCEPTION";
if (e == ExceptionCode::UNPRIVILEGED_EXCEPTION) return "UNPRIVILEGED_EXCEPTION";
if (e == ExceptionCode::INVALID_INPUT_EXCEPTION) return "INVALID_INPUT_EXCEPTION";
    throw std::invalid_argument("unknown enum");
  }

}  