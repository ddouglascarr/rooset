local seq = 0
local saved_tree

request.for_each(function()
  if seq == 0 then
    -- prepare for configuration/initialization phase
    trace._disabled = false
    trace._tree = { type = "root", db_time = 0 }
    trace._stack = { trace._tree }
    seq = 1
  elseif seq == 1 then
    -- save current state before processing first request
    if not (trace._stack[1] == trace._tree and trace._stack[2] == nil) then
      error("Trace stack unbalanced after initialization")
    end
    if not trace._disabled then
      saved_tree = table.new(trace._tree)
    end
    seq = 2
  elseif saved_tree then
    -- restore state before processing next request
    if saved_tree then
      trace._disabled = false
      trace._tree = table.new(saved_tree)
      trace._stack = { trace._tree }
    end
  end
end)
