
#pragma once
#include <string>
#include "enums/ContentType.h"
#include "enums/ExceptionCode.h"
#include "enums/DefeatStrength.h"
#include "enums/IssueState.h"
#include "enums/TieBreaking.h"
#include "enums/ExceptionType.h"

using namespace std;

namespace rooset {
  class EnumUtils
  {
  public:
    static ContentType parseContentType(const string& code);
static ExceptionCode parseExceptionCode(const string& code);
static DefeatStrength parseDefeatStrength(const string& code);
static IssueState parseIssueState(const string& code);
static TieBreaking parseTieBreaking(const string& code);
static ExceptionType parseExceptionType(const string& code);
    static string serializeContentType(ContentType e);
static string serializeExceptionCode(ExceptionCode e);
static string serializeDefeatStrength(DefeatStrength e);
static string serializeIssueState(IssueState e);
static string serializeTieBreaking(TieBreaking e);
static string serializeExceptionType(ExceptionType e);
  };
}

  