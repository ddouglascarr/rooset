#pragma once
#include <proxygen/httpserver/RequestHandler.h>
#include <r3/r3.hpp>
#include "HttpRoute.h"
#include "HttpRouteHandlerResponse.h"

namespace rooset {


class HttpRouter : public proxygen::RequestHandler
{
public:
  explicit HttpRouter(std::vector<rooset::HttpRoute> routes);

  void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
      noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;

private:
  std::unique_ptr<folly::IOBuf> body;
  std::unique_ptr<proxygen::HTTPMessage> request;
  std::vector<rooset::HttpRoute> routes;
  r3::Tree tree;
  static std::map<proxygen::HTTPMethod, int> httpMethodMap;
  
  void initTree();
};



}
