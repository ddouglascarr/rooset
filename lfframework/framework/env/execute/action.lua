--[[--
action_status =                    -- status code returned by action (a string), or, if "test_existence" == true, a boolean
execute.action{
  module         = module,         -- module name of the action to be executed
  action         = action,         -- name of the action to be executed
  id             = id,             -- id to be returned by param.get_id(...) during execution
  params         = params,         -- parameters to be returned by param.get(...) during execution
  test_existence = test_existence  -- do not execute action but only check if it exists
}

Executes an action without associated filters.

--]]--

function execute.action(args)
  local module = args.module
  local action = args.action
  local test   = args.test_existence
  if not test then
    trace.enter_action{ module = module, action = action }
  end
  local action_status = execute.file_path{
    file_path = encode.file_path(
      WEBMCP_BASE_PATH, 'app', WEBMCP_APP_NAME, module, '_action', action .. '.lua'
    ),
    id     = args.id,
    params = args.params,
    test_existence = test
  }
  if not test then
    trace.execution_return{ status = action_status }
  end
  return action_status
end
