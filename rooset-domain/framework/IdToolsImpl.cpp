#include "IdToolsImpl.h"

using namespace std;
using namespace boost::uuids;



rooset::IdToolsImpl::IdToolsImpl() :
    randomGenerator(nullptr),
    nilGenerator(nil_generator()),
    stringGenerator(string_generator())
{}



rooset::IdToolsImpl::IdToolsImpl(const IdToolsImpl& a) :
    randomGenerator(nullptr),
    nilGenerator(nil_generator()),
    stringGenerator(string_generator())
{}



uuid rooset::IdToolsImpl::generateUniqueId()
{
  if (randomGenerator == nullptr) {
    randomGenerator = make_unique<random_generator>();
  }
  return (*randomGenerator)();
}



uuid rooset::IdToolsImpl::generateNilId()
{
  return nilGenerator();
}



uuid rooset::IdToolsImpl::parse(string id)
{
  return stringGenerator(id);
}



string rooset::IdToolsImpl::serialize(uuid id)
{
  return boost::uuids::to_string(id);
}
