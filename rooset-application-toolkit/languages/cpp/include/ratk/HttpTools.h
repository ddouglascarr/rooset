#pragma once
#include <string>
#include <vector>

using namespace std;
namespace rooset {

  class HttpResponse
  {
  public:
    const long status;
    const string body;
    const vector<string> headers;

    inline HttpResponse(
        const long& status,
        const string& body) :
        status(status),
        body(body),
        headers()
    {}

    inline HttpResponse(
        const long& status,
        const string& body,
        const vector<string>& headers) :
        status(status),
        body(body),
        headers(headers)
    {}
  };



  class HttpTools
  {
  public:
    virtual ~HttpTools() {};

    virtual HttpResponse get(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url) =0;

    // for fully qualified uri
    virtual HttpResponse get(
        const vector<string>& headers,
        const string& uri) =0;

    virtual HttpResponse post(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url,
        const string& body) =0;
  };

}
