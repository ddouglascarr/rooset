#include "gtest/gtest.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <functional>
#include <r3/r3.hpp>
#include "http-server/HttpRoute.h"
#include "rapidjson/document.h"
#include <proxygen/httpserver/HTTPServer.h>
#include "http-server/HttpRouter.h"



TEST(dependencies, test_test_should_pass)
{
  EXPECT_EQ(true, true);
}



TEST(dependencies_lib3, should_match_route_c_api)
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

  r3_tree_free(n);
}



TEST(dependencies_lib3, should_match_route_cpp_api)
{
  r3::Tree tree(10);

  int routeData1 = 1;
  int routeData2 = 2;
  std::string routeFoo = "/foo";
  std::string routeFooXBar  = "/foo/{x}/bar";
  
  tree.insert_routel(METHOD_GET, routeFoo.c_str(), routeFoo.length(), &routeData1);
  tree.insert_routel(METHOD_GET, routeFooXBar.c_str(), routeFooXBar.length(), &routeData2);

  char* errStr;
  int err = tree.compile(&errStr);
  EXPECT_EQ(err, 0);
  if (err != 0) {
    free(errStr);
  }

  r3::MatchEntry fooEntry("/foo");
  fooEntry.set_request_method(METHOD_GET);
  
  r3::Route matchedFoo = tree.match_route(fooEntry);
  EXPECT_TRUE(matchedFoo);
  EXPECT_EQ(*static_cast<int*>(matchedFoo.data()), 1);
  
  r3::MatchEntry fooXBarEntry("/foo/123/bar");
  fooXBarEntry.set_request_method(METHOD_GET);

  r3::Route matchedFooXBar = tree.match_route(fooXBarEntry);
  EXPECT_TRUE(matchedFooXBar);
  EXPECT_EQ(*static_cast<int*>(matchedFooXBar.data()), 2);

  fooXBarEntry.set_request_method(METHOD_POST);
  r3::Route unmatched1 = tree.match_route(fooXBarEntry);
  EXPECT_FALSE(unmatched1);

  r3::MatchEntry unmatched2Entry("/foo/a/bar/123");
  unmatched2Entry.set_request_method(METHOD_GET);

  r3::Route unmatched2 = tree.match_route(unmatched2Entry);
  EXPECT_FALSE(unmatched2);
}



TEST(dependencies_lib3, pointer_to_vector_position)
{
  struct Route {
    std::string path;
    std::string value;
    std::function<std::string()> handler;
  };
  std::vector<Route> routes = { 
    {"/foo", "foo", []() { return "foo return value"; } }, 
    {"/bar", "bar", []() { return "bar return value"; } },
  };
  r3::Tree tree(routes.size());
  for (auto it = routes.begin(); it != routes.end(); ++it) {
    tree.insert_routel(METHOD_GET, it->path.c_str(), it->path.length(), &*it);
  }
  
  char* errStr;
  int err = tree.compile(&errStr);
  EXPECT_EQ(err, 0);
  if (err != 0) {
    free(errStr);
  }
  
  r3::MatchEntry barEntry("/bar");
  barEntry.set_request_method(METHOD_GET);
  r3::Route matched = tree.match_route(barEntry);
  EXPECT_TRUE(matched);

  auto* route = static_cast<Route*>(matched.data());
  EXPECT_EQ(route->value, "bar");
  EXPECT_EQ(route->handler(), "bar return value");       
}



TEST(dependencies_folly, covert_between_stdstring_and_fbstring)
{
  std::string stdStr("Foo Bar");
  folly::fbstring fbstr(stdStr);
  
  EXPECT_EQ(fbstr.toStdString(), "Foo Bar");
}
