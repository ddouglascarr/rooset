--[[--
request.configure(
  func              -- function which is performing the configuration
)

Registers a function to be called on every request only if a request is not in progress yet. Executes the function once in any case.

--]]--

function request.configure(func)
  if not request._in_progress then
    local initializers = request._initializers
    initializers[#initializers+1] = func
  end
  func()
end
