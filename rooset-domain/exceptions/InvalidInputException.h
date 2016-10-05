#pragma once
#include "ProjectException.h"

namespace rooset {
  class InvalidInputException : public ProjectException
  {
  private:
    const char* msg;
  public:
    InvalidInputException(char* msg) : ProjectException(msg) {};

    ExceptionCodes getExceptionCode() const override
    {
      return ExceptionCodes::INVALID_INPUT;
    }
  };
}