#include "gmock/gmock.h"
#include "ratk/IdToolsImpl.h"

using namespace rooset;



TEST(IdToolsImpl, generateUniqueId)
{
  IdToolsImpl idTools;

  auto id0 = idTools.generateUniqueId();
  auto id1 = idTools.generateUniqueId();
  auto id2 = idTools.generateUniqueId();

  EXPECT_NE(id0, idTools.generateNilId());
  EXPECT_NE(id1, idTools.generateNilId());
  EXPECT_NE(id2, idTools.generateNilId());

  EXPECT_NE(id0, id1);
  EXPECT_NE(id0, id2);
  EXPECT_NE(id1, id2);
}
