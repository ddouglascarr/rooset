function execute._initializers(initializer_path_element)
  local yieldvalue_inner = {}
  local coro = coroutine.create(function()
    execute.multi_wrapped(
      execute._create_sorted_execution_list(
        function(add_by_path)
          add_by_path(initializer_path_element)
          add_by_path(WEBMCP_APP_NAME, initializer_path_element)
        end,
        function(full_path, relative_path)
          execute.file_path{ file_path = full_path }
        end
      ),
      function()
        coroutine.yield(yieldvalue_inner)
      end
    )
  end)
  local success, yieldvalue = coroutine.resume(coro)
  if not success then
    error(debug.traceback(coro, "Error in initializer"))
  end
  if coroutine.status(coro) == "dead" then
    error("Initializers failed to call execute.inner()")
  elseif yieldvalue ~= yieldvalue_inner then
    error("Unexpected yield in initializer")
  end
  execute._finalizers[#execute._finalizers+1] = function()
    if not coroutine.resume(coro) then
      error(debug.traceback(coro, "Error in finalizer"))
    end
    if coroutine.status(coro) ~= "dead" then
      error("Unexpected yield in finalizer")
    end
  end
end
