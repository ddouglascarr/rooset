--[[--
path =                  -- string containing a path to a view
encode.view_file_path{
  module = module,      -- module name
  view   = view         -- view name
}

This function returns the file path of a view with a given module name and view name. Both module name and view name are mandatory arguments.

This function has been DEPRECATED and will be removed. To test whether a view exists, use execute.view{ test_existence=true, ... }.

--]]--

-- TODO: remove deprecated function

function encode.view_file_path(args)
  return (encode.file_path(
    WEBMCP_BASE_PATH, 'app', WEBMCP_APP_NAME, args.module, args.view .. '.lua'
  ))
end
