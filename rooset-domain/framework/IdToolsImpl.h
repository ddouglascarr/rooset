#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include "IdTools.h"

using namespace std;
using namespace boost::uuids;

namespace rooset {

  class IdToolsImpl : public IdTools
  {
  private:
    random_generator randomGenerator;
    nil_generator nilGenerator;
    string_generator stringGenerator;
   
  public:
    uuid generateUniqueId() override
    {
      return randomGenerator();
    }

    uuid generateNilId() override
    {
      return nilGenerator();
    }

    uuid parse(string id) override
    {
      return stringGenerator(id);
    }

    string serialize(uuid id) override
    {
      return boost::uuids::to_string(id);
    }

  };

}