--[[--
value =              -- cookie value, or nil if not set
request.get_cookie{
  name = name        -- name of cookie
}

Gets a cookie value.

--]]--

function request.get_cookie(args)
  return request._http_request.cookies[args.name]
end

--//--
