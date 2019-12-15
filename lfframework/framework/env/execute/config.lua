--[[--
execute.config(
  name           -- name of the configuration to be loaded
)

Executes a configuration file of the application.
This function is only used by by the mcp.lua file in the bin/ directory.

--]]--

function execute.config(name)
  trace.enter_config{ name = name }
  execute.file_path{
    file_path = encode.file_path(
      WEBMCP_BASE_PATH, 'config', name .. '.lua'
    )
  }
  trace.execution_return()
end
