--[[--
execute.inner()

It is mandatory to call this function once in each pre-fork and post-fork initializer and once in each filter of a WebMCP application.

Calling execute.inner() in an initializer marks the end of initialization (the remaining code is executed when the process is terminated cleanly).

Calling execute.inner() in a filter calls the next filter in the filter chain, or the view or action, if there are no more filters following. Code executed BEFORE calling this function is executed BEFORE the view or action, while code executed AFTER calling this function is executed AFTER the view of action.

--]]--

function execute.inner()
  local stack = execute._wrap_stack
  local pos = #stack
  if pos == 0 then
    error("Unexpected call of execute.inner().")
  end
  local inner_func = stack[pos]
  if not inner_func then
    error("Repeated call of execute.inner().")
  end
  stack[pos] = false
  inner_func()
end
