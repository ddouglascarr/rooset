--[[--
request.set_absolute_baseurl(
  url                          -- Base URL of the application
)

Calling this function is neccessary for every configuration, because an absolute URL is needed for HTTP redirects. If the URL of the application is volatile, and if you don't bother violating the HTTP standard, it is possible to skip calling this function in your configuration.

--]]--

function request.set_absolute_baseurl(url)
  request.configure(function()
    if string.find(url, "/$") then
      request._absolute_baseurl = url
    else
      request._absolute_baseurl = url .. "/"
    end
  end)
end
