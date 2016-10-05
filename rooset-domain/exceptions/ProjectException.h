#pragma once
#include <stdexcept>
#include "../enums/ExceptionCodes.h"

using namespace std;

namespace rooset {

  class ProjectException : public runtime_error
  {
  public:
    ProjectException(char* msg) : runtime_error(msg) {}

    virtual ExceptionCodes getExceptionCode() const = 0;
  };

}
