--[[--
request.initialize()

Executes all request initializers. Request initializers are added (and executed) using the request.for_each(...) call. Calling request.configure(...) before invoking request.initialize() for the first time also adds a request initializer.

--]]--
function request.initialize()
  _G.app = {}  -- may be filled and modified by request initializers
  do
    request._in_progress = true  -- NOTE: must be set to true before initializer functions are called
    for i, func in ipairs(request._initializers) do
      func()
    end
  end
end
--//--

--[[--
app  -- table to store an application state

'app' is a global table for storing any application state data. It will be reset for every request.
--]]--

-- Initialized in request.initialize(...).

--//--
