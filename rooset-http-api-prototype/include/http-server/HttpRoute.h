#pragma once

#include <string>
#include <map>
#include <functional>
#include <proxygen/httpserver/HTTPServer.h>
#include "HttpRouteHandlerResponse.h"

namespace rooset {



class HttpRoute
{
public:
  const bool isAuthenticated;
  const proxygen::HTTPMethod method;
  const std::string path;
  const std::function<HttpRouteHandlerResponse()> handler;

  inline HttpRoute(
      bool isAuthenticated,
      proxygen::HTTPMethod method,
      std::string path,
      decltype(handler) handler) :
      isAuthenticated(isAuthenticated),
      method(method),
      path(path),
      handler(handler) 
  {}

};



}
