#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include "enums/ExceptionCode.h"

using namespace std;
using namespace rooset;

namespace rooset {

  template<typename SerializationFormat>
  class ProjectException : public runtime_error
  {
  public:
    ProjectException(string msg) : runtime_error(msg.c_str()) {}
    virtual ~ProjectException() {};
    virtual unique_ptr<SerializationFormat> serialize() const = 0;

    virtual ExceptionCode getExceptionCode() const = 0;
  };

}
