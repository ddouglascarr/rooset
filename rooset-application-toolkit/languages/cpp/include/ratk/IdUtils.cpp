#include "IdUtils.h"



uuid rooset::IdUtils::generateNilId()
{
  return boost::uuids::nil_uuid();
}



uuid rooset::IdUtils::parse(string id)
{
  boost::uuids::string_generator gen;
  return gen(id);
}



string rooset::IdUtils::serialize(uuid id)
{
  return boost::uuids::to_string(id);
}
