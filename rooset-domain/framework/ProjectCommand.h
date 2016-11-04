#pragma once
#include <memory>
#include <string>

using namespace std;

namespace rooset {

  template<typename SerializationFormat>
  class ProjectCommand
  {
  public:
    virtual ~ProjectCommand() {};
	  virtual string getEventType() const = 0;
	  virtual unique_ptr<SerializationFormat> serialize() const = 0;
  };

}
