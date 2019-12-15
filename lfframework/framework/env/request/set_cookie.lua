--[[--
request.set_cookie{
  name   = name,     -- name of cookie
  value  = value,    -- value of cookie
  domain = domain,   -- optional domain domain where cookie is transmitted
  path   = path,     -- optional path where cookie is transmitted, defaults to application base
  secure = secure    -- optional boolean, indicating if cookie should only be transmitted over HTTPS
}

This function is similar to rocketwiki.set_cookie{...}, except that it automatically sets the path to the application base. It also sets secure=true, if the secure option is unset and the application base URL starts with "https://".

--]]--

function request.set_cookie(args)
  local args = table.new(args)
  if not args.path then
    args.path = string.match(
      request.get_absolute_baseurl(),
      "://[^/]*(.*)"
    )
    if args.path == nil then
      args.path = "/"
    end
  end
  if args.secure == nil then
    if string.find(
      string.lower(request.get_absolute_baseurl()),
      "^https://"
    ) then
      args.secure = true
    else
      args.secure = false
    end
  end
  assert(string.find(args.name, "^[0-9A-Za-z%%._~-]+$"), "Illegal cookie name")
  assert(string.find(args.value, "^[0-9A-Za-z%%._~-]+$"), "Illegal cookie value")
  local parts = {args.name .. "=" .. args.value}
  if args.domain then
    assert(
      string.find(args.path, "^[0-9A-Za-z%%/._~-]+$"),
      "Illegal cookie domain"
    )
    parts[#parts+1] = "domain=" .. args.domain
  end
  if args.path then
    assert(
      string.find(args.path, "^[0-9A-Za-z%%/._~-]+$"),
      "Illegal cookie path"
    )
    parts[#parts+1] = "path=" .. args.path
  end
  if args.secure then
    parts[#parts+1] = "secure"
  end
  request.add_header("Set-Cookie", table.concat(parts, "; "))
end
