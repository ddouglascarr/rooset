#include "gtest/gtest.h"

namespace roosetHttpTesting {



TEST(dependencies, test_test_should_pass)
{
  EXPECT_EQ(true, true);
}



TEST(dependencies, test_test_should_fail)
{
  EXPECT_EQ(true, false);
}


}
