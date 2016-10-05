#pragma once
#include "ProjectException.h"
#include "../enums/ExceptionCodes.h"

using namespace rooset;
namespace rooset {

  class UnauthorizedException : public ProjectException
  {
  private:
    const char* msg;
  public:
    UnauthorizedException(char* msg) : ProjectException(msg) {}

    ExceptionCodes getExceptionCode() const override
    {
      return ExceptionCodes::UNAUTHORIZED;
    }
  };

}
