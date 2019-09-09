--[[--
params =
request.get_param_strings{
  method           = method,           -- "GET", "POST", or nil to query both (POST has precedence)
  include_internal = include_internal  -- set to true to include also parameters starting with "_webmcp_" prefix
}

This function returns a table with all raw GET/POST parameters as strings or list of strings (except internal parameters like "_webmcp_path" or "_webmcp_id"). Modifications of the returned table have no side effects.

--]]--

local function merge_params(tbl, params_list, include)
  for key, values in pairs(params_list) do
    if not include and string.match(key, "^_webmcp_") then
      -- do nothing
    elseif string.match(key, "%[%]$") then
      tbl[key] = table.new(values)
    else
      tbl[key] = values[1]
    end
  end
end

function request.get_param_strings(args)
  local method = nil
  local include = false
  if args then
    method  = args.method
    include = args.include_internal
  end
  local t = {}
  if not method then
    merge_params(t, request._http_request.get_params_list, include)
    merge_params(t, request._http_request.post_params_list, include)
  elseif method == "GET" then
    merge_params(t, request._http_request.get_params_list, include)
  elseif method == "POST" then
    merge_params(t, request._http_request.post_params_list, include)
  else
    error("Invalid method passed to request.get_param_strings{...}")
  end
  return t
end
