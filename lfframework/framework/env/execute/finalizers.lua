--[[--
execute.finalizers()

Executes that code in initializers which is placed after execute.inner(). This function is automatically called when a forked process terminates. It may be called in interactive mode to perform a clean shutdown.

--]]--

function execute.finalizers()
  for i = #execute._finalizers, 1, -1 do
    execute._finalizers[i]()
    execute._finalizers[i] = nil
  end
end
