#!/usr/bin/env lua

-- module preamble
local _G, _M = _ENV, {}
_ENV = setmetatable({}, {
  __index = function(self, key)
    local value = _M[key]; if value ~= nil then return value end
    return _G[key]
  end,
  __newindex = _M
})

-- function that encodes certain HTML entities:
-- (not used by the library itself)
function encode_html(text)
  return (
    string.gsub(
      text, '[<>&"]',
      function(char)
        if char == '<' then
          return "&lt;"
        elseif char == '>' then
          return "&gt;"
        elseif char == '&' then
          return "&amp;"
        elseif char == '"' then
          return "&quot;"
        end
      end
    )
  )

end

-- function that encodes special characters for URIs:
-- (not used by the library itself)
function encode_uri(text)
  return (
    string.gsub(text, "[^0-9A-Za-z_%.~-]",
      function (char)
        return string.format("%%%02x", string.byte(char))
      end
    )
  )
end

-- function undoing URL encoding:
do
  local b0 = string.byte("0")
  local b9 = string.byte("9")
  local bA = string.byte("A")
  local bF = string.byte("F")
  local ba = string.byte("a")
  local bf = string.byte("f")
  function decode_uri(str)
    return (
      string.gsub(
        string.gsub(str, "%+", " "),
        "%%([0-9A-Fa-f][0-9A-Fa-f])",
        function(hex)
          local n1, n2 = string.byte(hex, 1, 2)
          if n1 >= b0 and n1 <= b9 then n1 = n1 - b0
          elseif n1 >= bA and n1 <= bF then n1 = n1 - bA + 10
          elseif n1 >= ba and n1 <= bf then n1 = n1 - ba + 10
          else error("Assertion failed") end
          if n2 >= b0 and n2 <= b9 then n2 = n2 - b0
          elseif n2 >= bA and n2 <= bF then n2 = n2 - bA + 10
          elseif n2 >= ba and n2 <= bf then n2 = n2 - ba + 10
          else error("Assertion failed") end
          return string.char(n1 * 16 + n2)
        end
      )
    )
  end
end

-- status codes that carry no response body (in addition to 1xx):
-- (set to "zero_content_length" if Content-Length header is required)
status_without_response_body = {
  ["101"] = true,  -- list 101 to allow protocol switch
  ["204"] = true,
  ["205"] = "zero_content_length",
  ["304"] = true
}

-- handling of GET/POST param tables:
local new_params_list  -- defined later
do
  local params_list_mapping = setmetatable({}, {__mode="k"})
  local function nextnonempty(tbl, key)
    while true do
      key = next(tbl, key)
      if key == nil then
        return nil
      end
      local value = tbl[key]
      if #value > 0 then
        return key, value
      end
    end
  end
  local function nextvalue(tbl, key)
    while true do
      key = next(tbl, key)
      if key == nil then
        return nil
      end
      local value = tbl[key][1]
      if value ~= nil then
        return key, value
      end
    end
  end
  local params_list_metatable = {
    __index = function(self, key)
      local tbl = {}
      self[key] = tbl
      return tbl
    end,
    __pairs = function(self)
      return nextnonempty, self, nil
    end
  }
  local params_metatable = {
    __index = function(self, key)
      return params_list_mapping[self][key][1]
    end,
    __newindex = function(self, key, value)
      params_list_mapping[self][key] = {value}
    end,
    __pairs = function(self)
      return nextvalue, params_list_mapping[self], nil
    end
  }
  -- function that returns a table to store key value-list pairs,
  -- and a second table automatically mapping keys to the first value
  -- using the key value-list pairs in the first table:
  new_params_list = function()
    local params_list = setmetatable({}, params_list_metatable)
    local params = setmetatable({}, params_metatable)
    params_list_mapping[params] = params_list
    return params_list, params
  end
end

-- function parsing URL encoded form data and storing it in
-- a key value-list pairs structure that has to be
-- previously obtained by calling by new_params_list():
local function read_urlencoded_form(tbl, data)
  for rawkey, rawvalue in string.gmatch(data, "([^?=&]*)=([^?=&]*)") do
    local subtbl = tbl[decode_uri(rawkey)]
    subtbl[#subtbl+1] = decode_uri(rawvalue)
  end
end

-- function to convert a HTTP request handler to a socket handler:
function generate_handler(handler, options)
  -- swap arguments if necessary (for convenience):
  if type(handler) ~= "function" and type(options) == "function" then
    handler, options = options, handler
  end
  -- helper function to process options:
  local function default(name, default_value)
    local value = options[name]
    if value == nil then
      return default_value
    else
      return value or nil
    end
  end
  -- process options:
  options = options or {}
  local preamble = ""  -- preamble sent with every(!) HTTP response
  do
    -- named arg "static_headers" is used to create the preamble:
    local s = options.static_headers
    local t = {}
    if s then
      if type(s) == "string" then
        for line in string.gmatch(s, "[^\r\n]+") do
          t[#t+1] = line
        end
      else
        for i, kv in ipairs(s) do
          if type(kv) == "string" then
            t[#t+1] = kv
          else
            t[#t+1] = kv[1] .. ": " .. kv[2]
          end
        end
      end
    end
    t[#t+1] = ""
    preamble = table.concat(t, "\r\n")
  end
  local input_chunk_size  = options.maximum_input_chunk_size  or options.chunk_size or 16384
  local output_chunk_size = options.minimum_output_chunk_size or options.chunk_size or 1024
  local header_size_limit = options.header_size_limit or 1024*1024
  local body_size_limit   = options.body_size_limit   or 64*1024*1024
  local idle_timeout           = default("idle_timeout", 65)
  local stall_timeout          = default("stall_timeout", 60)
  local request_header_timeout = default("request_header_timeout", 120)
  local response_timeout       = default("response_timeout", 3600)
  local drain_timeout          = default("drain_timeout", 2)
  local poll = options.poll_function or moonbridge_io.poll
  -- return socket handler:
  return function(socket)
    local socket_set = {[socket] = true}  -- used for poll function
    local survive = true  -- set to false if process shall be terminated later
    local consume  -- can be set to function that reads some input if possible
    -- function that may be used as "consume" function
    -- and which drains some input if possible:
    local function drain()
      local bytes, status = socket:drain_nb(input_chunk_size)
      if not bytes or status == "eof" then
        consume = nil
      end
    end
    -- function trying to unblock socket by reading:
    local function unblock()
      if consume then
        if not poll(socket_set, socket_set, stall_timeout) then
          socket:reset()
          error("Client connection stalled")
        end
        consume()
      else
        if not poll(nil, socket_set, stall_timeout) then
          socket:reset()
          error("Client connection stalled")
        end
      end
    end
    -- handle requests in a loop:
    repeat
      -- table for caching nil values:
      local headers_value_nil = {}
      -- create a new request object (methods are added later):
      local request  -- allow references to local variable
      request = {
        -- allow access to underlying socket:
        socket = socket,
        -- cookies are simply stored in a table:
        cookies = {},
        -- table mapping header field names to value-lists
        -- (raw access, but case-insensitive):
        headers = setmetatable({}, {
          __index = function(self, key)
            assert(type(key) == "string", "Attempted to index headers table with a non-string key")
            local lowerkey = string.lower(key)
            local result = rawget(self, lowerkey)
            if result == nil then
              result = {}
              rawset(self, lowerkey, result)
            end
            rawset(self, key, result)
            return result
          end
        }),
        -- table mapping header field names to value-lists
        -- (for headers with comma separated values):
        headers_csv_table = setmetatable({}, {
          __index = function(self, key)
            local result = {}
            for i, line in ipairs(request.headers[key]) do
              for entry in string.gmatch(line, "[^,]+") do
                local value = string.match(entry, "^[ \t]*(..-)[ \t]*$")
                if value then
                  result[#result+1] = value
                end
              end
            end
            self[key] = result
            return result
          end
        }),
        -- table mapping header field names to a comma separated string
        -- (for headers with comma separated values):
        headers_csv_string = setmetatable({}, {
          __index = function(self, key)
            local result = {}
            for i, line in ipairs(request.headers[key]) do
              result[#result+1] = line
            end
            result = table.concat(result, ", ")
            self[key] = result
            return result
          end
        }),
        -- table mapping header field names to a single string value
        -- (or false if header has been sent multiple times):
        headers_value = setmetatable({}, {
          __index = function(self, key)
            if headers_value_nil[key] then
              return nil
            end
            local values = request.headers_csv_table[key]
            if #values == 0 then
              headers_value_nil[key] = true
            else
              local result
              if #values == 1 then
                result = values[1]
              else
                result = false
              end
              self[key] = result
              return result
            end
          end
        }),
        -- table mapping header field names to a flag table,
        -- indicating if the comma separated value contains certain entries:
        headers_flags = setmetatable({}, {
          __index = function(self, key)
            local result = setmetatable({}, {
              __index = function(self, key)
                assert(type(key) == "string", "Attempted to index header flag table with a non-string key")
                local lowerkey = string.lower(key)
                local result = rawget(self, lowerkey) or false
                self[lowerkey] = result
                self[key] = result
                return result
              end
            })
            for i, value in ipairs(request.headers_csv_table[key]) do
              result[string.lower(value)] = true
            end
            self[key] = result
            return result
          end
        })
      }
      -- create metatable for request object:
      local request_mt = {}
      setmetatable(request, request_mt)
      -- callback for request body streaming:
      local process_body_chunk
      -- function to enable draining:
      local function enable_drain()
        consume = drain
        process_body_chunk = nil  -- allow for early garbage collection
      end
      -- local variables to track the state:
      local state = "init"        -- one of:
      --  "init"                  (initial state)
      --  "no_status_sent"        (request body streaming config complete)
      --  "info_status_sent"      (1xx status code has been sent)
      --  "bodyless_status_sent"  (204/304 status code has been sent)
      --  "status_sent"           (regular status code has been sent)
      --  "headers_sent"          (headers have been terminated)
      --  "finished"              (request has been answered completely)
      --  "faulty"                (I/O or protocaol error)
      local request_body_content_length  -- Content-Length of request body
      local close_requested = false  -- "Connection: close" requested
      local close_responded = false  -- "Connection: close" sent
      local content_length = nil     -- value of Content-Length header sent
      local bytes_sent = 0           -- number of bytes sent if Content-Length is set
      local chunk_parts = {}         -- list of chunks to send
      local chunk_bytes = 0          -- sum of lengths of chunks to send
      local streamed_post_params         = {}  -- mapping from POST field name to stream function
      local streamed_post_param_patterns = {}  -- list of POST field pattern and stream function pairs
      -- function to report an error:
      local function request_error(throw_error, status, text)
        local response_sent = false
        if
          state == "init" or
          state == "no_status_sent" or
          state == "info_status_sent"
        then
          local error_response_status, errmsg = pcall(function()
            request:monologue()
            request:send_status(status)
            request:send_header("Content-Type", "text/plain")
            request:send_data(status, "\n")
            if text then
              request:send_data("\n", text, "\n")
            end
            request:finish()
          end)
          if not error_response_status then
            if text then
              error("Error while sending error response (" .. status .. " / " .. text .. "): " .. errmsg)
            else
              error("Error while sending error response (" .. status .. "): " .. errmsg)
            end
          end
          response_sent = true
        end
        if throw_error then
          local errmsg
          if response_sent then
            errmsg = "Error while reading request from client. Error response: "
          else
            errmsg = "Error while reading request from client: "
          end
          errmsg = errmsg .. status
          if text then
            errmsg = errmsg .. " (" .. text .. ")"
          end
          error(errmsg)
        else
          return survive
        end
      end
      -- function that enforces consumption of all input:
      local function consume_all(timeout)
        local starttime = timeout and moonbridge_io.timeref()
        while consume do
          if timeout then
            -- passed timeout does not get reset but refers to starttime
            if not poll(socket_set, nil, timeout-moonbridge_io.timeref(starttime)) then
              return false
            end
          else
            -- stall_timeout gets reset for every poll
            if not poll(socket_set, nil, stall_timeout) then
              request_error(true, "408 Request Timeout", "Timeout while waiting for request body")
            end
          end
          consume()
        end
        return true
      end
      -- function to assert non-faulty handle:
      local function assert_not_faulty()
        assert(state ~= "faulty", "Tried to use faulty request handle")
      end
      -- functions to send data to the browser:
      local function send(...)
        local old_state = state; state = "faulty"
        local success, errmsg = socket:write_call(unblock, ...)
        if not success then
          socket:reset()
          error("Could not send data to client: " .. errmsg)
        end
        state = old_state
      end
      local function send_flush(...)
        local old_state = state; state = "faulty"
        local success, errmsg = socket:flush_call(unblock, ...)
        if not success then
          socket:reset()
          error("Could not send data to client: " .. errmsg)
        end
        state = old_state
      end
      -- function to assert proper finish/close/reset:
      local function assert_close(retval, errmsg)
        if not retval then
          error("Could not finish sending data to client: " .. errmsg)
        end
      end
      -- function to finish request:
      local function finish()
        if close_responded then
          -- discard any input:
          enable_drain()
          -- close output stream:
          send_flush()
          assert_close(socket:finish())
          -- wait for EOF from peer to avoid immediate TCP RST condition:
          if consume_all(drain_timeout) then
            -- fully close socket:
            assert_close(socket:close())
          else
            -- send TCP RST if draining input takes too long:
            assert_close(socket:reset())
          end
        else
          -- flush outgoing data:
          send_flush()
          -- consume incoming data:
          consume_all()
        end
      end
      -- function that writes out buffered chunks (without flushing the socket):
      function send_chunk()
        if chunk_bytes > 0 then
          local old_state = state; state = "faulty"
          send(string.format("%x\r\n", chunk_bytes))
          for i = 1, #chunk_parts do
            send(chunk_parts[i])
            chunk_parts[i] = nil
          end
          chunk_bytes = 0
          send("\r\n")
          state = old_state
        end
      end
      -- read functions
      local function read(...)
        local data, status = socket:read_yield(...)
        if data == nil then
          request_error(true, "400 Bad Request", "Read error")
        end
        if status == "eof" then
          request_error(true, "400 Bad Request", "Unexpected EOF")
        end
        return data
      end
      local function read_eof(...)
        local data, status = socket:read_yield(...)
        if data == nil then
          return nil  -- do not throw error on TCP RST
          --request_error(true, "400 Bad Request", "Read error")
        end
        if status == "eof" then
          if data == "" then
            return nil
          else
            request_error(true, "400 Bad Request", "Unexpected EOF")
          end
        end
        return data
      end
      -- reads a number of bytes from the socket,
      -- optionally feeding these bytes chunk-wise into
      -- the "process_body_chunk" callback function:
      local function read_body_bytes(remaining)
        while remaining > 0 do
          coroutine.yield()  -- do not read more than necessary
          local chunklen
          if remaining > input_chunk_size then
            chunklen = input_chunk_size
          else
            chunklen = remaining
          end
          local chunk = read(chunklen)
          remaining = remaining - chunklen
          if process_body_chunk then
            process_body_chunk(chunk)
          end
        end
      end
      -- coroutine for request body processing:
      local function read_body()
        if request.headers_flags["Transfer-Encoding"]["chunked"] then
          coroutine.yield()  -- do not read on first invocation
          local limit = body_size_limit
          while true do
            local line = read(32 + limit, "\n")
            local zeros, lenstr = string.match(line, "^(0*)([1-9A-Fa-f]+[0-9A-Fa-f]*)\r?\n$")
            local chunkext
            if lenstr then
              chunkext = ""
            else
              zeros, lenstr, chunkext = string.match(line, "^(0*)([1-9A-Fa-f]+[0-9A-Fa-f]*)([ \t;].-)\r?\n$")
            end
            if not lenstr or #lenstr > 13 then
              request_error(true, "400 Bad Request", "Encoding error while reading chunk of request body")
            end
            local len = tonumber("0x" .. lenstr)
            limit = limit - (#zeros + #chunkext + len)
            if limit < 0 then
              request_error(true, "413 Request Entity Too Large", "Request body size limit exceeded")
            end
            if len == 0 then break end
            read_body_bytes(len)
            local term = read(2, "\n")
            if term ~= "\r\n" and term ~= "\n" then
              request_error(true, "400 Bad Request", "Encoding error while reading chunk of request body")
            end
          end
          while true do
            local line = read(2 + limit, "\n")
            if line == "\r\n" or line == "\n" then break end
            limit = limit - #line
            if limit < 0 then
              request_error(true, "413 Request Entity Too Large", "Request body size limit exceeded while reading trailer section of chunked request body")
            end
          end
        elseif request_body_content_length then
          read_body_bytes(request_body_content_length)
        end
        if process_body_chunk then
          process_body_chunk(nil)  -- signal EOF
        end
        consume = nil  -- avoid further resumes
      end
      -- function to setup default request body handling:
      local function default_request_body_handling()
        local post_params_list, post_params = new_params_list()
        local content_type = request.headers_value["Content-Type"]
        if content_type then
          if
            content_type == "application/x-www-form-urlencoded" or
            string.match(content_type, "^application/x%-www%-form%-urlencoded *;")
          then
            read_urlencoded_form(post_params_list, request.body)
          else
            local boundary = string.match(
              content_type,
              '^multipart/form%-data[ \t]*[;,][ \t]*boundary="([^"]+)"$'
            ) or string.match(
              content_type,
              '^multipart/form%-data[ \t]*[;,][ \t]*boundary=([^"; \t]+)$'
            )
            if boundary then
              local post_metadata_list, post_metadata = new_params_list()
              boundary = "--" .. boundary
              local headerdata = ""
              local streamer
              local field_name
              local metadata = {}
              local value_parts
              local function default_streamer(chunk)
                value_parts[#value_parts+1] = chunk
              end
              local function stream_part_finish()
                if streamer == default_streamer then
                  local value = table.concat(value_parts)
                  value_parts = nil
                  if field_name then
                    local values = post_params_list[field_name]
                    values[#values+1] = value
                    local metadata_entries = post_metadata_list[field_name]
                    metadata_entries[#metadata_entries+1] = metadata
                  end
                else
                  streamer()
                end
                headerdata   = ""
                streamer     = nil
                field_name   = nil
                metadata     = {}
              end
              local function stream_part_chunk(chunk)
                if streamer then
                  streamer(chunk)
                else
                  headerdata = headerdata .. chunk
                  while true do
                    local line, remaining = string.match(headerdata, "^(.-)\r?\n(.*)$")
                    if not line then
                      break
                    end
                    if line == "" then
                      streamer = streamed_post_params[field_name]
                      if not streamer then
                        for i, rule in ipairs(streamed_post_param_patterns) do
                          if string.match(field_name, rule[1]) then
                            streamer = rule[2]
                            break
                          end
                        end
                      end
                      if not streamer then
                        value_parts = {}
                        streamer = default_streamer
                      end
                      streamer(remaining, field_name, metadata)
                      return
                    end
                    headerdata = remaining
                    local header_key, header_value = string.match(line, "^([^:]*):[ \t]*(.-)[ \t]*$")
                    if not header_key then
                      request_error(true, "400 Bad Request", "Invalid header in multipart/form-data part")
                    end
                    header_key = string.lower(header_key)
                    if header_key == "content-disposition" then
                      local escaped_header_value = string.gsub(header_value, '"[^"]*"', function(str)
                        return string.gsub(str, "=", "==")
                      end)
                      field_name = string.match(escaped_header_value, ';[ \t]*name="([^"]*)"')
                      if field_name then
                        field_name = string.gsub(field_name, "==", "=")
                      else
                        field_name = string.match(header_value, ';[ \t]*name=([^"; \t]+)')
                      end
                      metadata.file_name = string.match(escaped_header_value, ';[ \t]*filename="([^"]*)"')
                      if metadata.file_name then
                        metadata.file_name = string.gsub(metadata.file_name, "==", "=")
                      else
                        string.match(header_value, ';[ \t]*filename=([^"; \t]+)')
                      end
                    elseif header_key == "content-type" then
                      metadata.content_type = header_value
                    elseif header_key == "content-transfer-encoding" then
                      request_error(true, "400 Bad Request", "Content-transfer-encoding not supported by multipart/form-data parser")
                    end
                  end
                end
              end
              local skippart   = true   -- ignore data until first boundary
              local afterbound = false  -- interpret 2 bytes after boundary ("\r\n" or "--")
              local terminated = false  -- final boundary read
              local bigchunk = ""
              request:stream_request_body(function(chunk)
                if chunk == nil then
                  if not terminated then
                    request_error(true, "400 Bad Request", "Premature end of multipart/form-data request body")
                  end
                  request.post_params_list, request.post_params = post_params_list, post_params
                  request.post_metadata_list, request.post_metadata = post_metadata_list, post_metadata
                end
                if terminated then
                  return
                end
                bigchunk = bigchunk .. chunk
                while true do
                  if afterbound then
                    if #bigchunk <= 2 then
                      return
                    end
                    local terminator = string.sub(bigchunk, 1, 2)
                    if terminator == "\r\n" then
                      afterbound = false
                      bigchunk = string.sub(bigchunk, 3)
                    elseif terminator == "--" then
                      terminated = true
                      bigchunk = nil
                      return
                    else
                      request_error(true, "400 Bad Request", "Error while parsing multipart body (expected CRLF or double minus)")
                    end
                  end
                  local pos1, pos2 = string.find(bigchunk, boundary, 1, true)
                  if not pos1 then
                    if not skippart then
                      local safe = #bigchunk-#boundary
                      if safe > 0 then
                        stream_part_chunk(string.sub(bigchunk, 1, safe))
                        bigchunk = string.sub(bigchunk, safe+1)
                      end
                    end
                    return
                  end
                  if not skippart then
                    stream_part_chunk(string.sub(bigchunk, 1, pos1 - 1))
                    stream_part_finish()
                  else
                    boundary = "\r\n" .. boundary
                    skippart = false
                  end
                  bigchunk = string.sub(bigchunk, pos2 + 1)
                  afterbound = true
                end
              end)
              return  -- finalization is executed in stream handler
            else
              request_error(true, "415 Unsupported Media Type", "Unknown Content-Type of request body")
            end
          end
        end
        request.post_params_list, request.post_params = post_params_list, post_params
      end
      -- function to prepare body processing:
      local function prepare()
        assert_not_faulty()
        if state ~= "init" then
          return
        end
        if process_body_chunk == nil then
          default_request_body_handling()
        end
        if state ~= "init" then  -- re-check if state is still "init"
          return
        end
        consume = coroutine.wrap(read_body)
        consume()  -- call coroutine once to avoid hangup on empty body
        state = "no_status_sent"
        if request.headers_flags["Expect"]["100-continue"] then
          request:send_status("100 Continue")
          request:finish_headers()
        end
      end
      -- method to ignore input and close connection after response:
      function request:monologue()
        assert_not_faulty()
        if
          state == "headers_sent" or
          state == "finished"
        then
          error("All HTTP headers have already been sent")
        end
        local old_state = state; state = "faulty"
        enable_drain()
        close_requested = true
        if old_state == "init" then
          state = "no_status_sent"
        else
          state = old_state
        end
      end
      -- method to send a HTTP response status (e.g. "200 OK"):
      function request:send_status(status)
        prepare()
        local old_state = state; state = "faulty"
        if old_state == "info_status_sent" then
          send_flush("\r\n")
        elseif old_state ~= "no_status_sent" then
          state = old_state
          error("HTTP status has already been sent")
        end
        local status1 = string.sub(status, 1, 1)
        local status3 = string.sub(status, 1, 3)
        send("HTTP/1.1 ", status, "\r\n", preamble)
        local wrb = status_without_response_body[status3]
        if wrb then
          state = "bodyless_status_sent"
          if wrb == "zero_content_length" then
            request:send_header("Content-Length", 0)
          end
        elseif status1 == "1" then
          state = "info_status_sent"
        else
          state = "status_sent"
        end
      end
      -- method to send a HTTP response header:
      -- (key and value must be provided as separate args)
      function request:send_header(key, value)
        assert_not_faulty()
        if state == "init" or state == "no_status_sent" then
          error("HTTP status has not been sent yet")
        elseif
          state == "headers_sent" or
          state == "finished"
        then
          error("All HTTP headers have already been sent")
        end
        local old_state = state; state = "faulty"
        local key_lower = string.lower(key)
        if key_lower == "content-length" then
          if old_state == "info_status_sent" then
            state = old_state
            error("Cannot set Content-Length for informational status response")
          end
          local cl = assert(tonumber(value), "Invalid content-length")
          if content_length == nil then
            content_length = cl
          elseif content_length == cl then
            return
          else
            error("Content-Length has been set multiple times with different values")
          end
        elseif key_lower == "connection" then
          for entry in string.gmatch(string.lower(value), "[^,]+") do
            if string.match(entry, "^[ \t]*close[ \t]*$") then
              if old_state == "info_status_sent" then
                state = old_state
                error("Cannot set \"Connection: close\" for informational status response")
              end
              close_responded = true
              break
            end
          end
        end
        send(key, ": ", value, "\r\n")
        state = old_state
      end
      -- method to announce (and enforce) connection close after sending the
      -- response:
      function request:close_after_finish()
        assert_not_faulty()
        if state == "headers_sent" or state == "finished" then
          error("All HTTP headers have already been sent")
        end
        close_requested = true
      end
      -- function to terminate header section in response, optionally flushing:
      -- (may be called multiple times unless response is finished)
      local function finish_headers(with_flush)
        if state == "finished" then
          error("Response has already been finished")
        elseif state == "info_status_sent" then
          state = "faulty"
          send_flush("\r\n")
          state = "no_status_sent"
        elseif state == "bodyless_status_sent" then
          if close_requested and not close_responded then
            request:send_header("Connection", "close")
          end
          state = "faulty"
          send("\r\n")
          finish()
          state = "finished"
        elseif state == "status_sent" then
          if not content_length then
            request:send_header("Transfer-Encoding", "chunked")
          end
          if close_requested and not close_responded then
            request:send_header("Connection", "close")
          end
          state = "faulty"
          send("\r\n")
          if request.method == "HEAD" then
            finish()
          elseif with_flush then
            send_flush()
          end
          state = "headers_sent"
        elseif state ~= "headers_sent" then
          error("HTTP status has not been sent yet")
        end
      end
      -- method to finish and flush headers:
      function request:finish_headers()
        assert_not_faulty()
        finish_headers(true)
      end
      -- method to send body data:
      function request:send_data(...)
        assert_not_faulty()
        if state == "info_status_sent" then
          error("No (non-informational) HTTP status has been sent yet")
        elseif state == "bodyless_status_sent" then
          error("Cannot send response data for body-less status message")
        end
        finish_headers(false)
        if state ~= "headers_sent" then
          error("Unexpected internal status in HTTP engine")
        end
        if request.method == "HEAD" then
          return
        end
        state = "faulty"
        for i = 1, select("#", ...) do
          local str = tostring(select(i, ...))
          if #str > 0 then
            if content_length then
              local bytes_to_send = #str
              if bytes_sent + bytes_to_send > content_length then
                error("Content length exceeded")
              else
                send(str)
                bytes_sent = bytes_sent + bytes_to_send
              end
            else
              chunk_bytes = chunk_bytes + #str
              chunk_parts[#chunk_parts+1] = str
            end
          end
        end
        if chunk_bytes >= output_chunk_size then
          send_chunk()
        end
        state = "headers_sent"
      end
      -- method to flush output buffer:
      function request:flush()
        assert_not_faulty()
        send_chunk()
        send_flush()
      end
      -- method to finish response:
      function request:finish()
        assert_not_faulty()
        if state == "finished" then
          return
        elseif state == "info_status_sent" then
          error("Informational HTTP response can be finished with :finish_headers() method")
        end
        finish_headers(false)
        if state == "headers_sent" then
          if request.method ~= "HEAD" then
            state = "faulty"
            if content_length then
              if bytes_sent ~= content_length then
                error("Content length not used")
              end
            else
              send_chunk()
              send("0\r\n\r\n")
            end
            finish()
          end
          state = "finished"
        elseif state ~= "finished" then
          error("Unexpected internal status in HTTP engine")
        end
      end
      -- method to register POST param stream handler for a single field name:
      function request:stream_post_param(field_name, callback)
        if state ~= "init" then
          error("Cannot setup request body streamer at this stage anymore")
        end
        streamed_post_params[field_name] = callback
      end
      -- method to register POST param stream handler for a field name pattern:
      function request:stream_post_params(pattern, callback)
        if state ~= "init" then
          error("Cannot setup request body streamer at this stage anymore")
        end
        streamed_post_param_patterns[#streamed_post_param_patterns+1] = {pattern, callback}
      end
      -- method to register request body stream handler
      function request:stream_request_body(callback)
        if state ~= "init" then
          error("Cannot setup request body streamer at this stage anymore")
        end
        local inprogress = false
        local eof = false
        local buffer = {}
        process_body_chunk = function(chunk)
          if inprogress then
            if chunk == nil then
              eof = true
            else
              buffer[#buffer+1] = chunk
            end
          else
            inprogress = true
            callback(chunk)
            while #buffer > 0 do
              chunk = table.concat(buffer)
              buffer = {}
              callback(chunk)
            end
            if eof then
              callback()  -- signal EOF
            end
            inprogress = false
          end
        end
      end
      -- method to start reading request body
      function request:consume_input()
        prepare()
        consume_all()
      end
      -- method to stream request body
      function request:stream_request_body_now(callback)
        request:stream_request_body(function(chunk)
          if chunk ~= nil then
            callback(chunk)
          end
        end)
        request:consume_input()
      end
      -- metamethod to read special attibutes of request object:
      function request_mt:__index(key, value)
        if key == "faulty" then
          return state == "faulty"
        elseif key == "fresh" then
          return state == "init" and process_body_chunk == nil
        elseif key == "body" then
          local chunks = {}
          request:stream_request_body_now(function(chunk)
            chunks[#chunks+1] = chunk
          end)
          self.body = table.concat(chunks)
          return self.body
        elseif
          key == "post_params_list" or key == "post_params" or
          key == "post_metadata_list" or key == "post_metadata"
        then
          prepare()
          consume_all()
          return rawget(self, key)
        end
      end
      -- variable to store request target
      local target
      -- coroutine for reading headers:
      local function read_headers()
        -- initialize limit:
        local limit = header_size_limit
        -- read and parse request line:
        local line = read_eof(limit, "\n")
        if not line then
          return false, survive
        end
        limit = limit - #line
        if limit == 0 then
          return false, request_error(false, "414 Request-URI Too Long")
        end
        local proto
        request.method, target, proto =
          line:match("^([^ \t\r]+)[ \t]+([^ \t\r]+)[ \t]*([^ \t\r]*)[ \t]*\r?\n$")
        if not request.method then
          return false, request_error(false, "400 Bad Request")
        elseif proto ~= "HTTP/1.1" then
          return false, request_error(false, "505 HTTP Version Not Supported")
        end
        -- read and parse headers:
        while true do
          local line = read(limit, "\n");
          limit = limit - #line
          if line == "\r\n" or line == "\n" then
            break
          end
          if limit == 0 then
            return false, request_error(false, "431 Request Header Fields Too Large")
          end
          local key, value = string.match(line, "^([^ \t\r]+):[ \t]*(.-)[ \t]*\r?\n$")
          if not key then
            return false, request_error(false, "400 Bad Request")
          end
          local values = request.headers[key]
          values[#values+1] = value
        end
        return true  -- success
      end
      -- wait for input:
      local ready, pollmsg, quit = poll(socket_set, nil, idle_timeout, true)
      if not ready then
        if quit then
          return request_error(false, "408 Request Timeout", "Server shutdown")
        else
          return request_error(false, "408 Request Timeout", "Idle connection timed out")
        end
      end
      -- read headers (with timeout):
      do
        local coro = coroutine.wrap(read_headers)
        local starttime = request_header_timeout and moonbridge_io.timeref()
        while true do
          local status, retval = coro()
          if status == nil then
            local timeout
            if request_header_timeout then
              timeout = request_header_timeout - moonbridge_io.timeref(starttime)
              if stall_timeout and timeout > stall_timeout then
                timeout = stall_timeout
              end
            else
              timeout = stall_timeout
            end
            if not poll(socket_set, nil, timeout) then
              return request_error(false, "408 Request Timeout", "Timeout while receiving headers")
            end
          elseif status == false then
            return retval
          elseif status == true then
            break
          else
            error("Unexpected yield value")
          end
        end
      end
      -- process "Connection: close" header if existent:
      connection_close_requested = request.headers_flags["Connection"]["close"]
      -- process "Content-Length" header if existent:
      do
        local values = request.headers_csv_table["Content-Length"]
        if #values > 0 then
          request_body_content_length = tonumber(values[1])
          local proper_value = tostring(request_body_content_length)
          for i, value in ipairs(values) do
            value = string.match(value, "^0*(.*)")
            if value == "" then value = "0" end
            if value ~= proper_value then
              return request_error(false, "400 Bad Request", "Content-Length header(s) invalid")
            end
          end
          if request_body_content_length > body_size_limit then
            return request_error(false, "413 Request Entity Too Large", "Announced request body size is too big")
          end
        end
      end
      -- process "Transfer-Encoding" header if existent:
      do
        local flag = request.headers_flags["Transfer-Encoding"]["chunked"]
        local list = request.headers_csv_table["Transfer-Encoding"]
        if (flag and #list ~= 1) or (not flag and #list ~= 0) then
          return request_error(false, "400 Bad Request", "Unexpected Transfer-Encoding")
        end
      end
      -- process "Expect" header if existent:
      for i, value in ipairs(request.headers_csv_table["Expect"]) do
        if string.lower(value) ~= "100-continue" then
          return request_error(false, "417 Expectation Failed", "Unexpected Expect header")
        end
      end
      -- get mandatory Host header according to RFC 7230:
      request.host = request.headers_value["Host"]
      if not request.host then
        return request_error(false, "400 Bad Request", "No valid host header")
      end
      -- parse request target:
      request.path, request.query = string.match(target, "^/([^?]*)(.*)$")
      if not request.path then
        local host2
        host2, request.path, request.query = string.match(target, "^[Hh][Tt][Tt][Pp]://([^/?]+)/?([^?]*)(.*)$")
        if host2 then
          if request.host ~= host2 then
            return request_error(false, "400 Bad Request", "No valid host header")
          end
        elseif not (target == "*" and request.method == "OPTIONS") then
          return request_error(false, "400 Bad Request", "Invalid request target")
        end
      end
      -- parse GET params:
      request.get_params_list, request.get_params = new_params_list()
      if request.query then
        read_urlencoded_form(request.get_params_list, request.query)
      end
      -- parse cookies:
      for i, line in ipairs(request.headers["Cookie"]) do
        for rawkey, rawvalue in
          string.gmatch(line, "([^=; ]*)=([^=; ]*)")
        do
          request.cookies[decode_uri(rawkey)] = decode_uri(rawvalue)
        end
      end
      -- (re)set timeout for handler:
      timeout(response_timeout or 0)
      -- call underlying handler and remember boolean result:
      if handler(request) ~= true then survive = false end
      -- finish request (unless already done by underlying handler):
      request:finish()
      -- stop timeout timer:
      timeout(0)
    until close_responded
    return survive
  end
end

return _M

