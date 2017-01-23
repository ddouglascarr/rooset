#include "gmock/gmock.h"
#include "HttpTools.h"

namespace rooset {



class HttpToolsMockImpl :
    public rooset::HttpTools
{
public:
  MOCK_METHOD4(get, HttpResponse(
      const string& host,
      const string& port,
      const vector<string>& headers,
      const string& url));

  MOCK_METHOD2(get, HttpResponse(
      const vector<string>& headers,
      const string& uri));

  MOCK_METHOD5(post, HttpResponse(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url,
        const string& body));
};



}
