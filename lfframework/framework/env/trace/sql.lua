--[[--
trace.sql{
  command        = command,         -- executed SQL command as string
  execution_time = execution_time,  -- execution time of the statement in seconds
  error_position = error_position   -- optional position in bytes where an error occurred
}

This command is used to log SQL command execution. It does not need to be invoked manually.

--]]--

function trace.sql(args)
  if not trace._disabled then
    local command = args.command
    local execution_time = args.execution_time
    local error_position = args.error_position
    if type(command) ~= "string" then
      error("No command string passed to trace.sql{...}.")
    end
    if type(execution_time) ~= "number" then
      error("No execution time number passed to trace.sql{...}.")
    end
    if error_position and type(error_position) ~= "number" then
      error("error_position must be a number.")
    end
    trace._new_entry{
      type           = "sql",
      command        = command,
      execution_time = execution_time,
      error_position = error_position
    }
    for i, entry in ipairs(trace._stack) do
      entry.db_time = entry.db_time + execution_time
    end
  end
end
