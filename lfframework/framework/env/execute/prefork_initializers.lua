--[[--
execute.prefork_initializers()

Executes all initializers prior to forking.

--]]--

function execute.prefork_initializers()
  execute._initializers("_prefork")
end
