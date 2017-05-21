#include <gflags/gflags.h>

#include <future>
#include "EchoHandler.h"
#include "EchoStats.h"
#include "http-server/Server.h"
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>

using namespace EchoService;

class EchoHandlerFactory : public proxygen::RequestHandlerFactory
{
 public:
  void onServerStart(folly::EventBase* evb) noexcept override
  {
    stats_.reset(new EchoStats);
  }

  void onServerStop() noexcept override
  {
    stats_.reset();
  }

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler*, 
      proxygen::HTTPMessage*)
      noexcept override
  {
    return new EchoHandler(stats_.get());
  }

 private:
  folly::ThreadLocalPtr<EchoStats> stats_;
};



int main(int argc, char* argv[])
{
  rooset::Server<EchoHandlerFactory> server("localhost", 11000);
  
  std::promise<void> p;
  auto f = p.get_future();

  std::thread t = server.start([&p](){ p.set_value(); });
  
  f.get();
  LOG(INFO) << "server started \n";
  
  t.join();
  return 0;
}
