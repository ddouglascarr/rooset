#include "gmock/gmock.h"

#include <future>
#include "http-server/Server.h"
#include "EchoHandlerFactory.h"



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
