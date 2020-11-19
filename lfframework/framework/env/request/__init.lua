request._initializers = {}
request._in_progress = false

request._mime_types = {
  html = "text/html",
  css  = "text/css",
  js   = "application/javascript",
  png  = "image/png",
  jpeg = "image/jpeg",
  jpg  = "image/jpeg",
  gif  = "image/gif",
  svg  = "image/svg+xml",
  pdf  = "application/pdf",
  txt  = "text/plain"
}

request.for_each(function()
  request.data = json.object{}
  
  request._absolute_baseurl = nil
  request._response_headers = {}
  request._force_absolute_baseurl = false
  request._perm_params = {}
  request._404_route = nil
  request._cache = false
  request._cache_manual = false
  request._cache_time = 3600
  request._relative_baseurl = nil
  request._http_request = nil
  request._status = nil
  request._forward = nil
  request._forward_processed = false
  request._redirect = nil
  request._csrf_secret = nil

end)
