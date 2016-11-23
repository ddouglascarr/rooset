
#pragma once
#include <string>
#include "enums/ExceptionCode.h"
#include "enums/DefeatStrength.h"
#include "enums/TieBreaking.h"
#include "enums/ExceptionType.h"

using namespace std;

namespace rooset {
  class EnumUtils
  {
  public:
    static ExceptionCode parseExceptionCode(const string& code);
static DefeatStrength parseDefeatStrength(const string& code);
static TieBreaking parseTieBreaking(const string& code);
static ExceptionType parseExceptionType(const string& code);
    static string serializeExceptionCode(ExceptionCode e);
static string serializeDefeatStrength(DefeatStrength e);
static string serializeTieBreaking(TieBreaking e);
static string serializeExceptionType(ExceptionType e);
  };
}

  