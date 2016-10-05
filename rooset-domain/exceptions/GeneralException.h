#pragma once
#include "ProjectException.h"
#include "../enums/ExceptionCodes.h"

using namespace rooset;
namespace rooset {
  class GeneralException : public ProjectException
  {
  private:
    const char* msg;
  public:
    GeneralException(char* msg) : msg(msg) {}

    const char* what() const override
    {
      return msg;
    }

    ExceptionCodes getExceptionCode() const
    {
      return ExceptionCodes::GENERAL_ERROR;
    }

  };
}
