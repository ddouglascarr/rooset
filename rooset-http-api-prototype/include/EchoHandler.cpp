/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "EchoHandler.h"

#include <iostream>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include "EchoStats.h"

using namespace proxygen;
using namespace folly;

namespace EchoService {

EchoHandler::EchoHandler(EchoStats* stats): stats_(stats) {
}

void EchoHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {
  URL_ = headers->getURL();
  path_ = headers->getPath();
  stats_->recordRequest();
  request_ = move(headers);
}

void EchoHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  if (body_) {
    body_->prependChain(std::move(body));
  } else {
    body_ = std::move(body);
  }
}

void EchoHandler::onEOM() noexcept {
  auto bodyStr = body_->moveToFbString();
  bodyStr = "Hello " + bodyStr + ": " + path_;
  std::cout << bodyStr << "\n";
  std::cout << "What is the path: " + request_->getPath() + "\n";

  ResponseBuilder(downstream_)
    .status(200, "OK")
    .header("Request-Number",
            folly::to<std::string>(stats_->getRequestCount()))
    .body(bodyStr)
    .sendWithEOM();
}

void EchoHandler::onUpgrade(UpgradeProtocol protocol) noexcept {
  // handler doesn't support upgrades
}

void EchoHandler::requestComplete() noexcept {
  delete this;
}

void EchoHandler::onError(ProxygenError err) noexcept {
  delete this;
}

}
