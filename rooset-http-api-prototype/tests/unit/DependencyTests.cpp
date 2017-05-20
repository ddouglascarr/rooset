#include "gtest/gtest.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <r3/r3.h>



TEST(dependencies, test_test_should_pass)
{
  EXPECT_EQ(true, true);
}



TEST(dependencies_lib3, should_match_route)
{
  auto *n = r3_tree_create(10);
  int routeData1 = 1;
  int routeData2 = 2;
  std::string routeFoo = "/foo";
  std::string routeFooX = "/foo/{x}";
  
  r3_tree_insert_pathl(n, routeFoo.c_str(), routeFoo.length(), &routeData1);
  r3_tree_insert_pathl(n, routeFooX.c_str(), routeFooX.length(), &routeData2);

  char *errstr = nullptr;
  int err = r3_tree_compile(n, &errstr);
  if (err != 0) {
    std::cout << errstr << "\n";
    free(errstr);
  }
  EXPECT_EQ(err, 0);

  auto *matchedNode = r3_tree_matchl(n, "/foo", strlen("/foo"), NULL);
  EXPECT_TRUE(matchedNode);
  EXPECT_EQ((*(int*) matchedNode->data), 1);

  auto *matchedNodeBar = r3_tree_matchl(n, "/foo/abcd", strlen("/foo/abcd"), NULL);
  EXPECT_TRUE(matchedNodeBar);
  EXPECT_EQ((*(int*) matchedNodeBar->data), 2);

  auto* unmatched = r3_tree_matchl(n, "/bar", strlen("/bar"), NULL);
  EXPECT_FALSE(unmatched);
}

