--[[--
request.for_each(
  func             -- function to be called on every request
)

Registers a function to be called on every request and executes it once. This mechanism can be used for __init.lua files in the environment to perform a per-request initialization. See env/request/__init.lua for an example.

--]]--

function request.for_each(func)
  local initializers = request._initializers
  initializers[#initializers+1] = func
  func()
end
