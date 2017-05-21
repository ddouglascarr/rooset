#pragma once

#include "EchoHandler.h"
#include "EchoStats.h"
#include "http-server/Server.h"


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
    return new EchoHandler();
  }

 private:
  folly::ThreadLocalPtr<EchoStats> stats_;
};

