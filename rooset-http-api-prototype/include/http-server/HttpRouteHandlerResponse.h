#pragma once

#include <memory>
#include "rapidjson/document.h"



namespace rooset {



class HttpRouteHandlerResponse
{
public:
  const rapidjson::Document body;

  HttpRouteHandlerResponse(
      rapidjson::Document body) :
      body(std::move(body))
  {}
};



}
