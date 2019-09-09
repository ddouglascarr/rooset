--[[--
request.allow_caching()

Includes a "Cache-Control" header in the response that marks the content as cachable.

--]]--

function request.allow_caching()
  request.configure(function()
    request._cache = true
  end)
end
