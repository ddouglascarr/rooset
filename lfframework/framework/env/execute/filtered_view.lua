--[[--
execute.filtered_view{
  module = module,  -- module name of the view to be executed
  view   = view     -- name of the view to be executed
}

Executes a view with associated filters.

--]]--

function execute.filtered_view(args)
  execute.multi_wrapped(
    execute._create_sorted_execution_list(
      function(add_by_path)
        add_by_path("_filter")
        add_by_path("_filter_view")
        add_by_path(WEBMCP_APP_NAME, "_filter")
        add_by_path(WEBMCP_APP_NAME, "_filter_view")
        add_by_path(WEBMCP_APP_NAME, args.module, "_filter")
        add_by_path(WEBMCP_APP_NAME, args.module, "_filter_view")
      end,
      function(full_path, relative_path)
        trace.enter_filter{ path = relative_path }
        execute.file_path{ file_path = full_path }
        trace.execution_return()
      end
    ),
    function()
      execute.view(args)
    end
  )
end
