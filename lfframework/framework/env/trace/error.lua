--[[--
trace.error{
}

This function is called automatically in case of errors to log them.

--]]--

function trace.error(args)
  if not trace._disabled then
    trace._new_entry { type = "error" }
    while #trace._stack > 1 do
      trace._close_section()
    end
  end
end
