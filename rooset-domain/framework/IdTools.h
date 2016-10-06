#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>

using namespace std;
using namespace boost::uuids;

namespace rooset {
  class IdTools
  {
  public:
    virtual uuid generateNilId() = 0;
    virtual uuid generateUniqueId() = 0;
    virtual uuid parse(string) = 0;
    virtual string serialize(uuid) = 0;
  };
}