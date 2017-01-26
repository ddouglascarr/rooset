#pragma once
#include <string>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include "IdTools.h"
#include "IdToolsImpl.h"

using namespace std;

namespace rooset {
  using namespace boost::uuids;

  /*unique_ptr<IdTools> createIdToolsInstance()
  {
    return make_unique<IdToolsImpl>();
  }*/
}
