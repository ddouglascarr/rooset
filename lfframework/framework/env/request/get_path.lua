--[[--
path =
request.get_path()

Returns the path in the URL of the request. May return nil for a HTTP OPTIONS request with "*" target.

--]]--

function request.get_path()
  return request._http_request.path
end
