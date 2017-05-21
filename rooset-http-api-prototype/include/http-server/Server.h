#pragma once

#include <vector>
#include <future>
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <unistd.h>

namespace rooset {



template<typename HandlerFactory>
class Server
{
private:
  std::vector<proxygen::HTTPServer::IPConfig> ipConfigs;
  proxygen::HTTPServer s;

public:



  inline Server(std::string serverAddress, int port) :
      s(generateOptions(sysconf(_SC_NPROCESSORS_ONLN)))
  {
    ipConfigs = {
      { 
        folly::SocketAddress(serverAddress, port, true),
        proxygen::HTTPServer::Protocol::HTTP
      },
    };

  }



  ~Server() {}



  std::thread start(std::function<void()> callback)
  {
    s.bind(ipConfigs);
    std::thread t([&, this] () {
      s.start(callback);
    });

    return t;
  }



  void stop()
  {
    s.stop();
  }



private:



  proxygen::HTTPServerOptions generateOptions(int threads)
  {
    proxygen::HTTPServerOptions options;
    options.threads = static_cast<size_t>(threads);
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = false;
    options.handlerFactories = proxygen::RequestHandlerChain()
        .addThen<HandlerFactory>()
        .build();
    options.h2cEnabled = true;
    return options;
  }



};



}
