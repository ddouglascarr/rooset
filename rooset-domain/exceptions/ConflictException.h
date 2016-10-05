#pragma once
#include "ProjectException.h"
#include "../enums/ExceptionCodes.h"

namespace rooset {
  class ConflictException : public rooset::ProjectException
  {
  private:
    const char* msg;
  public:

    ConflictException(char* msg) : msg(msg) {}
    
    const char* what() const override
    {
      return msg;
    }

    ExceptionCodes getExceptionCode() const override
    {
      return rooset::ExceptionCodes::CONFLICT;
    }

  };
}