--[[--
route =
request.default_router(
  path                   -- URL path, including a leading slash
)

Default conversion from a path to a route. Called by request.router().

--]]--

function request.default_router(path)
  if not path then
    return nil
  end
  if path == "" then
    return {module = "index", view = "index"}
  end
  local static = string.match(path, "^static/([-./0-9A-Z_a-z]*)$")
  if static then
    -- Note: sanitizer is in request.handler(...)
    return {static = static}
  end
  local module, action, view, id, suffix
  module = string.match(path, "^([^/]+)/$")
  if module then
    return {module = module, view = "index"}
  end
  module, action = string.match(path, "^([^/]+)/([^/.]+)$")
  if module then
    return {module = module, action = action}
  end
  module, view, suffix = string.match(path, "^([^/]+)/([^/.]+)%.([^/]+)$")
  if module then
    return {module = module, view = view, suffix = suffix}
  end
  module, view, id, suffix = string.match(path, "^([^/]+)/([^/]+)/([^/.]+)%.([^/]+)$")
  if module then
    return {module = module, view = view, id = id, suffix = suffix}
  end
  return nil
end

--//--
