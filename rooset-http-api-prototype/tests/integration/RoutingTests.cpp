#include "gmock/gmock.h"

#include <future>
#include <memory>
#include "http-server/Server.h"
#include "EchoHandlerFactory.h"
#include "ratk/HttpToolsCurlImpl.h"



TEST(routing_tests, server_should_start_and_stop)
{
  // starting server
  rooset::Server<EchoHandlerFactory> server("localhost", 11005);
  std::promise<void> p;
  auto f = p.get_future();
  std::thread t = server.start([&p](){ p.set_value(); });
  
  // wait for server to be ready before starting tests
  f.get();
  EXPECT_EQ(true, true);

  // kill the server, and wait for the thread to close
  server.stop();
  t.join();
}



TEST(routing_tests, echo_server)
{
  // starting server
  rooset::Server<EchoHandlerFactory> server("localhost", 11005);
  std::promise<void> p;
  auto f = p.get_future();
  std::thread t = server.start([&p](){ p.set_value(); });
  
  // setup curl
  rooset::HttpToolsCurlImpl httpTools;

  // wait for server to be ready before starting tests
  f.get();
  auto resp = httpTools.get("localhost", "11005", {}, "/foo");
  EXPECT_EQ(resp.body, "Hello : /foo");


  // kill the server, and wait for the thread to close
  server.stop();
  t.join();

}
