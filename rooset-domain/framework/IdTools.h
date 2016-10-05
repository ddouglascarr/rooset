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
    virtual uuid parseStringId(string) = 0;
  };
}