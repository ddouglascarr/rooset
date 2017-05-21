#include <future>
#include "EchoHandlerFactory.h"
#include "http-server/Server.h"



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
