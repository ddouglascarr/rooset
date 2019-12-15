-- Moonbridge example application
-- invoke with ./moonbridge example_application.lua
--
-- see helloworld.lua for a simpler example

local http = require "moonbridge_http"

-- preparation before forking:
local documents = {}
for i, document_name in ipairs{"example_webpage.html", "example_webpage.css"} do
  local file = assert(io.open(document_name))
  documents[document_name] = file:read("*a")  -- store file contents in memory
  file:close()
end

listen{
  -- listen to a tcp version 4 socket
  --{ proto = "tcp", host = "0.0.0.0", port = 8080 },

  -- listen to a tcp version 6 socket
  { proto = "tcp", host = "::", port = 8080},

  -- listen to a unix domain socket
  --{ proto = "local", path = 'socket' },

  -- execute the listener regularly (without incoming connection)
  --{ proto = "interval", name = "myint", delay = 10, strict = false },

  -- desired number of spare (idle) processes
  pre_fork = 1, -- number of forks

  -- minimum number of processes
  min_fork = 4, -- number of forks

  -- maximum number of processes (hard limit)
  max_fork = 16, -- number of forks

  -- delay between creation of spare processes
  fork_delay = 0.25, -- seconds

  -- delay before retry of failed process creation
  fork_error_delay = 2, -- seconds

  -- delay between destruction of excessive spare processes
  exit_delay = 60, -- seconds

  -- idle time after a fork gets terminated
  idle_timeout = 0, -- seconds (0 for no timeout)

  -- maximum memory consumption before process gets terminated
  --memory_limit = 1024*1024, -- bytes
  
  -- preparation of process (executed after fork)
  prepare = function()
    -- e.g. open database connection
  end,
  
  -- connection handler
  connect = http.generate_handler(
    {
      static_headers = {"Server: Moonbridge Example Server"},
      request_header_size_limit = 1024*1024,          -- maximum size of request headers
      request_body_size_limit   = 16*1024*1024*1024,  -- allow big file uploads
      idle_timeout              = 65,     -- maximum time until receiving the first byte of the request headera
      stall_timeout             = 60,     -- maximum time a client connection may be stalled
      request_header_timeout    = 120,    -- maximum time until receiving the remaining bytes of the request header
      response_timeout          = 3600,   -- time in which request body and response must be sent
      maximum_input_chunk_size  = 16384,  -- tweaks behavior of request-body parser
      minimum_output_chunk_size = 1024    -- chunk size for chunked-transfer-encoding
    },
    function(request)

      local function error_response(status)
        request:send_status(status)
        request:send_header("Content-Type", "text/html")
        request:send_data("<html>\n<head><title>", status, "</title></head>\n<body><h1>", status, "</h1></body>\n</html>\n")
        request:finish()
      end

      if request.method == "GET" or request.method == "HEAD" then

        if request.path == "" then
          request:send_status("303 See Other")
          request:send_header("Location", "http://" .. request.headers_value.host .. "/example_webpage.html")

        else
          local document_name = request.path
          local document_extension = string.match(document_name, "%.([^.])$")
          local document = documents[document_name]  -- loads file contents from memory
          if document then
            request:send_status("200 OK")
            if document_extension == "html" then
              request:send_header("Content-Type", "text/html; charset=UTF-8")
            elseif document_extension == "css" then
              request:send_header("Content-Type", "text/css; charset=UTF-8")
            end
            request:send_data(document)
          else
            error_response("404 Not Found")
          end

        end

      elseif request.method == "POST" then

        if request.path == "post_example" then
          local files = {}
          do
            local file
            request:stream_post_param("files", function(chunk, field_name, meta)
              if meta then
                file = {
                  file_name = meta.file_name,
                  content_type = meta.content_type,
                  length = 0
                }
              end
              if chunk then
                file.length = file.length + #chunk
              else
                files[#files+1] = file
              end
            end)
          end
          
          request:send_status("200 OK")
          request:send_header("Content-Type", "text/html; charset=UTF-8")
          request:send_data("<html>\n<head>\n")
          request:send_data('<link href="example_webpage.css" rel="stylesheet" type="text/css">\n')
          request:send_data("<title>Moonbridge Network Server for Lua Applications &ndash; Example Application</title>\n")
          request:send_data("</head>\n<body>\n")
          request:send_data("<h1>Moonbridge Network Server for Lua &ndash; Example Application</h1>\n")
          request:send_data("<h2>POST request successful</h2>\n")
          request:send_data('<table>\n<thead><th>File name</th><th>Content type</th><th class="numeric">Bytes received</th></thead>\n<tbody>\n')
          request:consume_input()
          for i, file in ipairs(files) do
            request:send_data("<tr>")
            request:send_data("<td>", http.encode_html(file.file_name or "(unknown)"), "</td>")
            request:send_data("<td>", http.encode_html(file.content_type or "(unknown)"), "</td>")
            request:send_data('<td class="numeric">', http.encode_html(tostring(file.length)), "</td>")
            request:send_data("</tr>\n")
          end
          request:send_data("</tbody>\n</table>\n")
          request:send_data("<p>Submitted comment: ", http.encode_html(request.post_params.comment), "</p>\n")
          request:send_data("</body>\n</html>\n")

        else
          error_response("404 Not Found")

        end
        
      else
        error_response("405 Method not allowed")

      end

      -- returning false causes termination of current process (and re-forking)
      return true
    end),
    
  -- executed on process termination
  finish = function()
    -- e.g. close database connection
  end
}

