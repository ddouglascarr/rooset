#include "HttpRouter.h"
#include <proxygen/httpserver/ResponseBuilder.h>
#include "ratk/ProjectException.h"
#include "enums/ExceptionCode.h"

namespace rooset {



HttpRouter::HttpRouter(std::vector<rooset::HttpRoute> routes) :
    routes(routes),
    tree(routes.size())
{
  initTree();
}



void HttpRouter::onRequest(
    std::unique_ptr<proxygen::HTTPMessage> req) noexcept
{
  request = move(req);
}



void HttpRouter::onBody( std::unique_ptr<folly::IOBuf> b ) noexcept
{
  if (body) {
    body->prependChain(std::move(b));
  } else {
    body = std::move(b);
  }
}



void HttpRouter::onEOM() noexcept
{
  // TODO: handle the request. EOD
  try {

  } catch (const rooset::ProjectException<rapidjson::Document>& e) {

  } catch (const std::exception& e) {

  }
}



void HttpRouter::onUpgrade(proxygen::UpgradeProtocol) noexcept
{
  // no support for upgrades
}



void HttpRouter::requestComplete() noexcept
{
  delete this;
}



void HttpRouter::onError(proxygen::ProxygenError) noexcept
{
  delete this;
}



void HttpRouter::initTree()
{
  for (auto it = routes.begin(); it != routes.end(); ++it) {
    tree.insert_routel(
        httpMethodMap.at(it->method),
        it->path.c_str(),
        it->path.length(),
        &*it);
  }

  char* errStr;
  int err = tree.compile(&errStr);
  if (err != 0) {
    // TODO: what do I do about this?
    LOG(ERROR) << "Tree copilation failed: " << errStr << "\n";
    free(errStr);
    return;
  }

}



std::map<proxygen::HTTPMethod, int> HttpRouter::httpMethodMap {
  { proxygen::HTTPMethod::GET, METHOD_GET },
  { proxygen::HTTPMethod::POST, METHOD_POST },
  { proxygen::HTTPMethod::PUT, METHOD_PUT },
  { proxygen::HTTPMethod::DELETE, METHOD_DELETE },
};



}
