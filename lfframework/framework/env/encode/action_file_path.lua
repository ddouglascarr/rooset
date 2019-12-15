--[[--
path =                    -- string containing a path to an action
encode.action_file_path{
  module = module,        -- module name
  action = action         -- action name
}

This function returns the file path of an action with a given module name and action name. Both module name and action name are mandatory arguments.

This function has been DEPRECATED and will be removed. To test whether an action exists, use execute.action{ test_existence=true, ... }.

--]]--

-- TODO: remove deprecated function

function encode.action_file_path(args)
  return (encode.file_path(
    WEBMCP_BASE_PATH,
    'app',
    WEBMCP_APP_NAME,
    args.module,
    '_action',
    args.action .. '.lua'
  ))
end
