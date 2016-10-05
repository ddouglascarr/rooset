#pragma once
#include "ProjectException.h"
#include "../enums/ExceptionCodes.h"

namespace rooset {
  class ConflictException : public rooset::ProjectException
  {
  private:
    const char* msg;
  public:

    ConflictException(char* msg) : ProjectException(msg) {}
    
    ExceptionCodes getExceptionCode() const override
    {
      return rooset::ExceptionCodes::CONFLICT;
    }

  };
}