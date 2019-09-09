--[[--
view_exists =                      -- boolean returned if "test_existence" is set to true, otherwise no value returned
execute.view{
  module         = module,         -- module name of the view to be executed
  view           = view,           -- name of the view to be executed
  id             = id,             -- id to be returned by param.get_id(...) during execution
  params         = params,         -- parameters to be returned by param.get(...) during execution
  test_existence = test_existence  -- do not execute view but only check if it exists
}

Executes a view directly (without associated filters).

--]]--

function execute.view(args)
  local module = args.module
  local view   = args.view
  local test   = args.test_existence
  if not test then
    trace.enter_view{ module = module, view = view }
  end
  local result = execute.file_path{
    file_path = encode.file_path(
      WEBMCP_BASE_PATH, 'app', WEBMCP_APP_NAME, module, view .. '.lua'
    ),
    id     = args.id,
    params = args.params,
    test_existence = test
  }
  if not test then
    trace.execution_return()
  end
  if test then
    return result
  end
end
