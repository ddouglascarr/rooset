#pragma once
#include <string>
#include <memory>
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
    unique_ptr<random_generator> randomGenerator = nullptr;
    nil_generator nilGenerator;
    string_generator stringGenerator;
   
  public:
    IdToolsImpl();
    IdToolsImpl(const IdToolsImpl& a);
    uuid generateUniqueId() override;
    uuid generateNilId() override;
    uuid parse(string id) override;
    string serialize(uuid id) override;
  };

}
