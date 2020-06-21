#!/usr/bin/env moonbridge

--[[--
WEBMCP_VERSION

A string containing the WebMCP version, e.g. "2.0.0"
--]]--
WEBMCP_VERSION = "2.1.0"
--//--

--[[--
WEBMCP_MODE

A constant set to "listen" in case of a network request, or set to "interactive" in case of interactive mode.
--]]--
if _MOONBRIDGE_VERSION then
  WEBMCP_MODE = "listen"
else
  WEBMCP_MODE = "interactive"
end
--//--

--[[--
WEBMCP_CONFIG_NAMES

A list of the selected configuration names.
--]]--
-- configuration names are provided as 4th, 5th, etc. command line argument
WEBMCP_CONFIG_NAMES = {}
--//--

--[[--
WEBMCP_FRAMEWORK_PATH

Directory of the WebMCP framework (always includes a trailing slash).
--]]--
-- set in mcp.lua
--//--

--[[--
WEBMCP_BASE_PATH

Base directory of the application (always includes a trailing slash).
--]]--
-- set in mcp.lua
--//--

--[[--
WEBMCP_APP_NAME

Application name (usually "main"). May be nil in case of interactive mode.
--]]--
-- set in mcp.lua
--//--

-- determine framework and bath path from command line arguments
-- or print usage synopsis (if applicable)
do
  local arg1, arg2, arg3, arg4, arg5 = ...
  local helpout
  if
    arg1 == "-h" or arg1 == "--help" or
    arg2 == "-h" or arg2 == "--help"  -- if first arg is provided by wrapper
  then
    helpout = io.stderr
  end
  if helpout then
    helpout:write("Usage: moonbridge [moonbr opts] -- <framework path>/bin/mcp.lua <framework path> <app base path> <app name> <config name> \n")
    helpout:write("   or: lua [-i]    [Lua opts]    -- <framework path>/bin/mcp.lua <framework path> <app base path> <app name> <config name> [<lua_file>]\n")
    if helpout == io.stderr then
      return 1
    else
      return 0
    end
  end
  local function append_trailing_slash(str)
    return (string.gsub(str, "([^/])$", function(last) return last .. "/" end))
  end
  WEBMCP_FRAMEWORK_PATH = append_trailing_slash(arg1)
  WEBMCP_BASE_PATH      = append_trailing_slash(arg2)
  WEBMCP_APP_NAME       = arg3
  WEBMCP_CONFIG_NAMES   = {arg4}
  WEBMCP_INTERACTIVE_FILE = arg5
end

-- setup search paths for libraries
do
  if string.match(package.path, "^[^;]") then
    package.path = ";" .. package.path
  end
  package.path = WEBMCP_FRAMEWORK_PATH .. "lib/?.lua" .. package.path
  -- find out which file name extension shared libraries have
  local slib_exts = {}
  for ext in string.gmatch(package.cpath, "%?%.([A-Za-z0-9_-]+)") do
    if not slib_exts[ext] then
      slib_exts[#slib_exts+1] = ext
      slib_exts[ext] = true
    end
  end
  local paths = {}
  for i, ext in ipairs(slib_exts) do
    paths[#paths+1] = WEBMCP_FRAMEWORK_PATH .. "accelerator/?." .. ext
  end
  for i, ext in ipairs(slib_exts) do
    paths[#paths+1] = WEBMCP_FRAMEWORK_PATH .. "lib/?." .. ext
  end
  paths[#paths+1] = package.cpath
  package.cpath = table.concat(paths, ";")
end

-- load "extos" library (needed by function "loadcached")
_G.extos = require "extos"

--[[--
_G

A reference to the global namespace. To avoid accidental programming errors, global variables cannot be set directly, but they must be set through the _G reference, e.g. use _G.foo = true to set the variable "foo" to a value of true.

Note that the global namespace may or may not be shared between requests (Moonbridge creates multiple forks of the Lua machine). To set variables that are to be cleared after the request has been finished, an application may use the "app" table, e.g. app.foo = true to set the variable app.foo to a value of true, which will be cleared automatically when the request has ended.

--]]--
local _G = _G
local allowed_globals = {}
local protected_environment = setmetatable(
  {},  -- proxy environment used all chunks loaded through loadcached(...)
  {
    __index = _G,
    __newindex = function(self, key, value)
      if allowed_globals[key] then
        _G[key] = value
      else
        if type(key) == "string" and string.match(key, "^[A-Za-z_][A-Za-z_0-9]*$") then
          error('Attempt to set global variable "' .. key .. '" (Hint: missing local statement? Use _G.' .. key .. '=<value> to really set global variable.)', 2)
        else
          error('Attempt to set global variable', 2)
        end
      end
    end
  }
)
--//--

--[[--
lua_func,    -- compiled Lua function, nil if the file does not exist
errmsg =     -- error message (only for non-existing file, other errors are thrown)
loadcached(
  filename   -- path to a Lua source or byte-code file
)

Loads, compiles and caches a Lua chunk. The cached value (i.e. the compiled function) is returned. If the file does not exist, nil and an error string are returned. Any other errors are thrown using error(...). Unsuccessful attempts are not cached (to prohibit cache pollution).

--]]--
do
  local cache = {}
  function loadcached(filename)
    local cached_func = cache[filename]
    if cached_func then
      return cached_func
    end
    local stat, errmsg = extos.stat(filename)
    if stat == nil then
      error(errmsg)
    elseif stat == false then
      return nil, 'File "' .. filename .. '" does not exist'
    elseif stat.isdir then
      error('File "' .. filename .. '" is a directory')
    elseif not stat.isreg then
      error('File "' .. filename .. '" is not a regular file')
    end
    local func, compile_error = loadfile(filename, nil, protected_environment)
    if func then
      cache[filename] = func
      return func
    end
    error(compile_error, 0)
  end
end
--//--

-- check if framework path is correct
do
  local file, errmsg = io.open(WEBMCP_FRAMEWORK_PATH .. "webmcp_version", "r")
  if not file then
    error('Could not find "webmcp_version" file: ' .. errmsg, 0)
  end
  local version = assert(file:read())
  assert(file:close())
  if version ~= WEBMCP_VERSION then
    error('Version mismatch in file "' .. WEBMCP_FRAMEWORK_PATH .. 'webmcp_version"')
  end
end

-- autoloader system for WebMCP environment "$WEBMCP_FRAMEWORK_PATH/env/",
-- application environment extensions "$WEBMCP_BASE_PATH/env/"
-- and models "$WEBMCP_BASE_PATH/model/"
do
  local weakkey_mt = { __mode = "k" }
  local autoloader_category = setmetatable({}, weakkey_mt)
  local autoloader_path     = setmetatable({}, weakkey_mt)
  local autoloader_mt       = {}
  local function install_autoloader(self, category, path_fragment)
    autoloader_category[self] = category
    autoloader_path[self]     = path_fragment
    setmetatable(self, autoloader_mt)
  end
  local function try_exec(filename)
    local func = loadcached(filename)
    if func then
      func()
      return true
    else
      return false
    end
  end
  function autoloader_mt.__index(self, key)
    local category, base_path, merge_base_path, file_key
    local merge = false
    if
      string.find(key, "^[a-z_][A-Za-z0-9_]*$") and
      not string.find(key, "^__")
    then
      category        = "env"
      base_path       = WEBMCP_FRAMEWORK_PATH .. "env/"
      merge           = true
      merge_base_path = WEBMCP_BASE_PATH .. "env/"
      file_key        = key
    elseif string.find(key, "^[A-Z][A-Za-z0-9]*$") then
      category        = "model"
      base_path       = WEBMCP_BASE_PATH .. "model/"
      local first = true
      file_key = string.gsub(key, "[A-Z]",
        function(c)
          if first then
            first = false
            return string.lower(c)
          else
            return "_" .. string.lower(c)
          end
        end
      )
    else
      return
    end
    local required_category = autoloader_category[self]
    if required_category and required_category ~= category then return end
    local path_fragment = autoloader_path[self]
    local path = base_path .. path_fragment .. file_key
    local merge_path
    if merge then
      merge_path = merge_base_path .. path_fragment .. file_key
    end
    local function try_dir(dirname)
      local dir = io.open(dirname)
      if dir then
        io.close(dir)
        local obj = {}
        install_autoloader(obj, category, path_fragment .. file_key .. "/")
        rawset(self, key, obj)
        try_exec(path .. "/__init.lua")
        if merge then try_exec(merge_path .. "/__init.lua") end
        return true
      else
        return false
      end
    end
    if self == _G then
      allowed_globals[key] = true
    end
    if merge and try_exec(merge_path .. ".lua") then
    elseif merge and try_dir(merge_path .. "/") then
    elseif try_exec(path .. ".lua") then
    elseif try_dir(path .. "/") then
    else end
    if self == _G then
      allowed_globals[key] = nil
    end
    return rawget(self, key)
  end
  install_autoloader(_G, nil, "")
  try_exec(WEBMCP_FRAMEWORK_PATH .. "env/__init.lua")
  try_exec(WEBMCP_BASE_PATH .. "env/__init.lua")
end

-- define post-fork initialization function (including loading of "multirand" library)
local function postfork_init()
  multirand = require "multirand"
  execute.postfork_initializers()
end

--[[--
listen{
  {
    proto     = proto,            -- "local", "tcp", or "interval"
    path      = path,             -- path to unix domain socket if proto == "local"
    port      = port,             -- TCP port number
    host      = host,             -- "::" for all IPv6 interfaces, "0.0.0.0" for all IPv4 interfaces
    name      = interval_name,    -- optional interval name (may be useful for log output)
    handler   = interval_handler  -- interval handler if proto == "interval"
  },
  {
    ...                           -- second listener
  },
  ...                             -- more listeners
  -- the following options are all optional and have default values:
  pre_fork         = pre_fork,          -- desired number of spare (idle) processes
  min_fork         = min_fork,          -- minimum number of processes
  max_fork         = max_fork,          -- maximum number of processes (hard limit)
  fork_delay       = fork_delay,        -- delay (seconds) between creation of spare processes
  fork_error_delay = fork_error_delay,  -- delay (seconds) before retry of failed process creation
  exit_delay       = exit_delay,        -- delay (seconds) between destruction of excessive spare processes
  idle_timeout     = idle_timeout,      -- idle time (seconds) after a fork gets terminated (0 for no timeout)
  memory_limit     = memory_limit,      -- maximum memory consumption (bytes) before process gets terminated
  min_requests_per_fork = min_requests_per_fork,  -- minimum count of requests handled before fork is terminated
  max_requests_per_fork = max_requests_per_fork,  -- maximum count of requests handled before fork is terminated
  http_options = {
    static_headers            = static_headers,             -- string or table of static headers to be returned with every request
    request_header_size_limit = request_header_size_limit,  -- maximum size of request headers sent by client
    request_body_size_limit   = request_body_size_limit,    -- maximum size of request body sent by client
    idle_timeout              = idle_timeout,               -- maximum time until receiving the first byte of the request header
    stall_timeout             = stall_timeout,              -- maximum time a client connection may be stalled
    request_header_timeout    = request_header_timeout,     -- maximum time until receiving the remaining bytes of the request header
    response_timeout          = response_timeout,           -- time in which request body and response must be sent
    maximum_input_chunk_size  = maximum_input_chunk_size,   -- tweaks behavior of request-body parser
    minimum_output_chunk_size = minimum_output_chunk_size   -- chunk size for chunked-transfer-encoding
  }
}

The listen{...} function determines on which TCP port an application is answering requests. A typical call looks as follows:

listen{
  { proto = "tcp4", port = 8080, localhost = true },
  { proto = "tcp6", port = 8080, localhost = true }
}

This function must be called in a configuration file (in the config/ directory) or in pre-fork initializers (in the app/_prefork/ or app/<application name>/_prefork/ directories), unless WebMCP is invoked in interactive mode (in which case any calls of listen{...} are ignored).

This function is a variant of Moonbridge's listen{...} function which has been wrapped for WebMCP. No "prepare", "conenct", or "finish" handler can be set. Instead WebMCP automatically dispatches incoming connections. For interval timers, an interval handler may be specified in each listener.

--]]--
-- prepare for interactive or listen mode
if WEBMCP_MODE == "interactive" then
  function listen()  -- overwrite Moonbridge's listen function
    -- ignore listen function calls for interactive mode
  end
  trace.disable()  -- avoids memory leakage when scripts are running endlessly
else
  local moonbridge_listen = listen
  local http = require("moonbridge_http")
  function listen(args)  -- overwrite Moonbridge's listen function
    assert(args, "No argument passed to listen function")
    local min_requests_per_fork = args.min_requests_per_fork or 50
    local max_requests_per_fork = args.max_requests_per_fork or 200
    local interval_handlers = {}
    for j, listener in ipairs(args) do
      if listener.proto == "interval" then
        local name = listener.name or "Unnamed interval #" .. #interval_handlers+1
        if interval_handlers[name] ~= nil then
          error('Interval handler with duplicate name "' .. name .. '"')
        end
        interval_handlers[name] = listener.handler
        listener.name = name
      end
    end
    local request_count = 0
    local function inner_handler(http_request)
      request_count = request_count + 1
      if request_count >= max_requests_per_fork then
        http_request:close_after_finish()
      end
      request.initialize()
      return request.handler(http_request)
    end
    local outer_handler = http.generate_handler(inner_handler, args.http_options)
    args.prepare = postfork_init
    args.connect = function(socket)
      if socket.interval then
        request_count = request_count + 1
        request.initialize()
        interval_handlers[socket.interval]()
      else
        local success = outer_handler(socket)
        if not success then
          return false
        end
      end
      return request_count < min_requests_per_fork
    end
    args.finish = execute.finalizers
    moonbridge_listen(args)
  end
end
--//--

-- execute configurations and pre-fork initializers
for i, config_name in ipairs(WEBMCP_CONFIG_NAMES) do
  execute.config(config_name)
end
execute.prefork_initializers()

-- perform post-fork initializations (once) in case of interactive mode
if WEBMCP_MODE == "interactive" then
  postfork_init()
  if WEBMCP_INTERACTIVE_FILE then
    dofile(WEBMCP_INTERACTIVE_FILE)
  end
end
