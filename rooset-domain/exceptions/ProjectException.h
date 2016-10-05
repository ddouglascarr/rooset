#pragma once
#include <stdexcept>
#include "../enums/ExceptionCodes.h"

using namespace std;

namespace rooset {

  class ProjectException : public runtime_error
  {
  public:
    virtual const char* what() const override = 0;
    virtual ExceptionCodes getExceptionCode() const = 0;
  };

}
