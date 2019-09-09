--[[--
value =                 -- value of GET/POST parameter, or value list if multiple == true
request.get_param{
  method   = method,    -- "GET", "POST", or nil to query both (POST has precedence)
  name     = name,      -- field name
  index    = index,     -- defaults to 1 to get first occurrence, only applicable if multiple == false
  multiple = multiple,  -- boolean to indicate whether to return a single value or a value list
  meta     = meta       -- set to true to get metadata (table with "file_name" and "content_type")
}

Reads GET/POST parameters directly from the underlaying request.

--]]--

function request.get_param(args)
  local param_list
  if args.metadata then
    if args.method == "GET" then
      error("HTTP GET parameters do not have metadata")
    elseif args.method == "POST" or not args.method then
      param_list = request._http_request.post_metadata_list[args.name]
    else
      error("Unknown HTTP method selected")
    end
  else
    if args.method == "GET" then
      param_list = request._http_request.get_params_list[args.name]
    elseif args.method == "POST" then
      param_list = request._http_request.post_params_list[args.name]
    elseif not args.method then
      param_list = request._http_request.post_params_list[args.name]
      if not param_list[args.index or 1] then
        param_list = request._http_request.get_params_list[args.name]
      end
    else
      error("Unknown HTTP method selected")
    end
  end
  if args.multiple then
    return param_list
  else
    return param_list[args.index or 1]
  end
end

--//--
