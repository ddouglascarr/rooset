#include "HttpToolsCurlImpl.h"

#include <cstdlib>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include "exceptions/CommandEvaluationException.h"



rooset::HttpToolsCurlImpl::HttpToolsCurlImpl()
{
  if (!hasGlobalInitRun) {
    curl_global_init(CURL_GLOBAL_ALL);
    std::atexit(curl_global_cleanup);
    hasGlobalInitRun = true;
  }
}



rooset::HttpResponse rooset::HttpToolsCurlImpl::get(
    const string& host,
    const string& port,
    const vector<string>& headers,
    const string& url)
{
  auto curlhost = "http://" + host + ":" + port + url;
  return get(headers, curlhost);
}



rooset::HttpResponse rooset::HttpToolsCurlImpl::get(
        const vector<string>& headers,
        const string& uri)
{
  auto conn = curl_easy_init();
  std::string body;
  std::vector<string> respHeaders;
  char errorBuffer[CURL_ERROR_SIZE];
  long status = 0;
  CURLcode code;

  if (conn == NULL) throw runtime_error("Curl will not easy init");

  code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set error buffer");

  code = curl_easy_setopt(conn, CURLOPT_URL, uri.c_str());
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set url: " + uri);

  struct curl_slist* curlHeaders=NULL;
  for (auto& header : headers) {
    curlHeaders = curl_slist_append(curlHeaders, header.c_str());
  }

  code = curl_easy_setopt(conn, CURLOPT_HTTPHEADER, curlHeaders);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set headers");

  code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set writer");

  code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &body);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set write data");

  code = curl_easy_setopt(
      conn,
      CURLOPT_HEADERFUNCTION,
      [](void *buffer, size_t size, size_t nmemb, void *userp) {
        char *d = static_cast<char*>(buffer);
        auto* h = static_cast<vector<string>*>(userp);
        
        int result = 0;
        if (h != nullptr) {
          std::string header = "";
          header.append(d, size * nmemb);
          h->push_back(header);
          result = size * nmemb;
        }
        return result;
      });
  assertCurlCodeOk(conn, code, errorBuffer, "failed to register header resp function");

  code = curl_easy_setopt(conn, CURLOPT_WRITEHEADER, &respHeaders);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to register header resp variable");

  code = curl_easy_perform(conn);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to perform request");

  code = curl_easy_getinfo(conn, CURLINFO_RESPONSE_CODE, &status);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to get status code");

  curl_easy_reset(conn);

  return HttpResponse(status, body);
}



rooset::HttpResponse rooset::HttpToolsCurlImpl::post(
        const string& host,
        const string& port,
        const vector<string>& headers,
        const string& url,
        const string& reqBody)
{
  auto conn = curl_easy_init();
  std::string body;
  char errorBuffer[CURL_ERROR_SIZE];
  long status = 0;
  CURLcode code;

  if (conn == NULL) throw runtime_error("Curl will not easy init");

  code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set error buffer");

  auto curlhost = "http://" + host + ":" + port + url;
  code = curl_easy_setopt(conn, CURLOPT_URL, curlhost.c_str());
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set url: " + curlhost);

  struct curl_slist* curlHeaders=NULL;
  for (auto& header : headers) {
    curlHeaders = curl_slist_append(curlHeaders, header.c_str());
  }

  code = curl_easy_setopt(conn, CURLOPT_HTTPHEADER, curlHeaders);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set headers");

  code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set writer");

  code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &body);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set write data");

  code = curl_easy_setopt(conn, CURLOPT_POSTFIELDS, reqBody.c_str());
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set post body");

  code = curl_easy_setopt(conn, CURLOPT_POSTFIELDSIZE, reqBody.size());
  assertCurlCodeOk(conn, code, errorBuffer, "failed to set post body size");

  code = curl_easy_perform(conn);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to perform request");

  code = curl_easy_getinfo(conn, CURLINFO_RESPONSE_CODE, &status);
  assertCurlCodeOk(conn, code, errorBuffer, "failed to get status code");

  curl_easy_reset(conn);

  return HttpResponse(status, body);
}



void rooset::HttpToolsCurlImpl::assertCurlCodeOk(
    CURL*& conn, CURLcode code, char errorBuffer[CURL_ERROR_SIZE], string message)
{
  if (code != CURLE_OK) {
    curl_easy_reset(conn);
    throw std::runtime_error("curl: " + message + "\n" + string(errorBuffer));
  }
}



size_t rooset::HttpToolsCurlImpl::writer(
    char* buffer, size_t size, size_t nmemb, std::string* userp)
{
  if (userp == NULL) return 0;
  userp->append(buffer, size * nmemb);
  return size * nmemb;
}



bool rooset::HttpToolsCurlImpl::hasGlobalInitRun = false;
