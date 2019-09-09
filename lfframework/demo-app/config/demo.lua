-- uncomment the following two lines to use C implementations of chosen
-- functions and to disable garbage collection during the request, to
-- increase speed:
--
-- require 'webmcp_accelerator'
-- collectgarbage("stop")

-- open and set default database handle
config.db = {
  engine='postgresql',
  dbname='webmcp_demo'
}

listen{
  { proto = "tcp4", port = 8080 },
  { proto = "tcp6", port = 8080 },
  { proto = "interval", delay = 60, handler = function() io.stderr:write("Background job executed here\n") end }
}

--[[
-- enable output of SQL commands in trace system
function db:sql_tracer(command)
  return function(error_info)
    local error_info = error_info or {}
    trace.sql{ command = command, error_position = error_info.position }
  end
end
--]]

-- 'request.get_relative_baseurl()' should be replaced by the absolute
-- base URL of the application, as otherwise HTTP redirects will not be
-- standard compliant
--request.set_absolute_baseurl(request.get_relative_baseurl())

-- uncomment the following lines, if you want to use a database driven
-- tempstore (for flash messages):
--
-- function tempstore.save(blob)
--   return Tempstore:create(blob)
-- end
-- function tempstore.pop(key)
--   return Tempstore:data_by_key(key)
-- end


