--[[--
route =
request.router()

Returns a table with routing information for a request. The table contains the mandatory field "module", one of "view" or "action", and may contain the optional fields "id" and "suffix". This function may be overwritten by an application and calls request.default_router(request.get_path()) in its default implementation.

--]]--

function request.router()
  return (request.default_router(request.get_path()))
end

--//--
