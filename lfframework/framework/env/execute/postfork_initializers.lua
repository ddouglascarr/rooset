--[[--
execute.postfork_initializers()

Executes all initializers after forking.

--]]--

function execute.postfork_initializers()
  execute._initializers("_postfork")
end
