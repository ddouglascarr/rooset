#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

using namespace std;
using namespace boost::uuids;

namespace rooset {

  class IdUtils
  {
  public:
    static uuid generateNilId();
    static uuid parse(string id);
    static string serialize(uuid id);
  };
}
