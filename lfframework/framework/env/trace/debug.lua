--[[--
trace.debug(
  value1,     -- value to be converted to a string and included in the debug output
  value2,     -- another value to be converted to a string and included in the debug output
  ...
)


This function can be used to include debug output in the trace log. Each argument is converted to a string (using tostring(...)) and all results are concatenated with a single space character between them.

--]]--

function trace.debug(...)
  if not trace._disabled then
    local values = {}
    for i = 1, select("#", ...) do
      values[i] = tostring((select(i, ...)))
    end
    trace._new_entry{ type = "debug", message = table.concat(values, " ") }
  end
end
