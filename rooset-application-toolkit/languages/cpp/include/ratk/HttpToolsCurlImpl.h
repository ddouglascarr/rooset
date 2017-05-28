#include "HttpTools.h"
#include <curl/curl.h>

using namespace std;
namespace rooset {
  class HttpToolsCurlImpl :
    public HttpTools
  {
  public:
    HttpToolsCurlImpl();

    HttpResponse get(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url) override;

    HttpResponse get(
        const vector<string>& headers,
        const string& uri) override;

    HttpResponse post(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url,
        const string& body) override;

  protected:
    void assertCurlCodeOk(
        CURL*& conn, CURLcode code, char errorBuffer[CURL_ERROR_SIZE], string message);

    static size_t writer(
        char* buffer, size_t size, size_t nmemb, std::string* userp);

    static size_t headerWriter(
        char* buffer, size_t size, size_t nmemb, void* userp);

    static bool hasGlobalInitRun;
  };
}
