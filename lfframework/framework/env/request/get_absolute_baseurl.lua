--[[--
baseurl =
request.get_absolute_baseurl()

This function returns the absolute base URL of the application, as set by request.set_absolute_baseurl(...). As a fallback (if no absolute base URL has been set), this function will return the relative base URL.

--]]--

function request.get_absolute_baseurl()
  if request._absolute_baseurl then
    return request._absolute_baseurl
  else
    return request._relative_baseurl
  end
end
