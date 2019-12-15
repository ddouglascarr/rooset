--[[--
request.set_cache_time(
  seconds                -- duration in seconds
)

Sets the expiration timeout for static content.

--]]--

function request.set_cache_time(seconds)
  request.configure(function()
    request._cache_time = seconds
  end)
end
