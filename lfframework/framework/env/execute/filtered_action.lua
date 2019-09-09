--[[--
action_status =     -- status code returned by the action (a string)
execute.filtered_action{
  module = module,  -- module name of the action to be executed
  action = action,  -- name of the action to be executed
  id     = id,      -- id to be returned by param.get_id(...) during execution
  params = params   -- parameters to be returned by param.get(...) during execution
}

Executes an action with associated filters.

--]]--

function execute.filtered_action(args)
  local result
  execute.multi_wrapped(
    execute._create_sorted_execution_list(
      function(add_by_path)
        add_by_path("_filter")
        add_by_path("_filter_action")
        add_by_path(WEBMCP_APP_NAME, "_filter")
        add_by_path(WEBMCP_APP_NAME, "_filter_action")
        add_by_path(WEBMCP_APP_NAME, args.module, "_filter")
        add_by_path(WEBMCP_APP_NAME, args.module, "_filter_action")
      end,
      function(full_path, relative_path)
        trace.enter_filter{ path = relative_path }
        execute.file_path{ file_path = full_path }
        trace.execution_return()
      end
    ),
    function()
      result = execute.action(args)
    end
  )
  return result
end
