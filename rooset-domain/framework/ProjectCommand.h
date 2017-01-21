#pragma once
#include <memory>
#include <string>
#include "rapidjson/document.h"

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


inline bool operator== (
    const rooset::ProjectCommand<rapidjson::Document>& a,
    const rooset::ProjectCommand<rapidjson::Document>& b)
{
  return a.serialize() == b.serialize();
}

inline bool operator!= (
    const rooset::ProjectCommand<rapidjson::Document>& a,
    const rooset::ProjectCommand<rapidjson::Document>& b)
{
  return !(a == b);
}
