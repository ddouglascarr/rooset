#include <gflags/gflags.h>

#include "EchoHandler.h"
#include "EchoStats.h"
#include "http-server/Server.h"
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>

using namespace EchoService;
using namespace proxygen;

class EchoHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* evb) noexcept override {
    stats_.reset(new EchoStats);
  }

  void onServerStop() noexcept override {
    stats_.reset();
  }

  RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
    return new EchoHandler(stats_.get());
  }

 private:
  folly::ThreadLocalPtr<EchoStats> stats_;
};

int main(int argc, char* argv[]) {
  rooset::Server<EchoHandlerFactory> server("localhost", 11000);

  auto t = server.start();
  t.join();
  return 0;
}
