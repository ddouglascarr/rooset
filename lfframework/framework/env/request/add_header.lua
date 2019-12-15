--[[--
request.add_header(
  key,               -- name of header, e.g. "Date"
  value              -- value, e.g. "Mon, 1 Jan 2001 01:00:00 GMT"
)

Adds a HTTP header to the response.

--]]--

function request.add_header(key, value)
  if value == nil then
    error("Function request.add_header(...) requires two arguments")
  end
  request.configure(function()
    local headers = request._response_headers
    headers[#headers+1] = {key, value}
    local lower_key = string.lower(key)
    if lower_key == "cache-control" then
      request._cache_manual = true
    end
  end)
end
