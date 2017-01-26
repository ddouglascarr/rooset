#pragma once
#include "gmock/gmock.h"
#include "IdToolsImpl.h"

namespace rooset {



class IdToolsMockImpl :
    public IdToolsImpl
{
public:
  using IdToolsImpl::generateNilId;
  using IdToolsImpl::parse;
  using IdToolsImpl::serialize;

  IdToolsMockImpl() : IdToolsImpl() {};

  MOCK_METHOD0(generateUniqueId, uuid());
};



}
