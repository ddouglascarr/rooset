#include "gmock/gmock.h"

#include <future>
#include <memory>
#include "http-server/Server.h"
#include "EchoHandler.h"
#include "ratk/HttpToolsCurlImpl.h"



class RoutingTestsFixture : public ::testing::Test
{
public:
  const std::string HOST = "localhost";
  const int PORT = 11005;
  rooset::HttpToolsCurlImpl httpTools;
  class TestHandlerFactory;

  RoutingTestsFixture() : httpTools() {};
  ~RoutingTestsFixture() {};

  template<typename S>
  inline std::thread startServer(S& server)
  {
    std::promise<void> p;
    auto f = p.get_future();
    std::thread t = server.start([&p](){ p.set_value(); });
    
    f.get();
    return t;
  }
};



class RoutingTestsFixture::TestHandlerFactory : 
    public proxygen::RequestHandlerFactory
{
public:
  void onServerStart(folly::EventBase* evb) noexcept override {}

  void onServerStop() noexcept override {}

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler*, 
      proxygen::HTTPMessage*)
      noexcept override
  {
    return new EchoService::EchoHandler();
  }

};



TEST_F(RoutingTestsFixture, server_should_start_and_stop)
{
  // starting server
  rooset::Server<TestHandlerFactory> server("localhost", 11005);
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



TEST_F(RoutingTestsFixture, echo_server)
{
  rooset::Server<TestHandlerFactory> server(HOST, PORT);
  auto t = startServer<decltype(server)>(server);

  auto resp = httpTools.get(HOST, std::to_string(PORT), {}, "/foo");
  EXPECT_EQ(resp.body, "Hello : /foo");


  // kill the server, and wait for the thread to close
  server.stop();
  t.join();
}
