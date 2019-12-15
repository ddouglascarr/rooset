-- minimal example application for Moonbridge
-- invoke with ./moonbridge helloworld.lua
--
-- see example_application.lua for a more elaborated example

local http = require "moonbridge_http"

listen{
  { proto = "tcp", host = "127.0.0.1", port = 8080 },  -- IPv4
  { proto = "tcp", host = "::1",       port = 8080 },  -- IPv6
  connect = http.generate_handler(
    function(request)
      local function error_response(status)
        request:send_status(status)
        request:send_header("Content-Type", "text/html")
        request:send_data("<html>\n")
        request:send_data("<head><title>", status, "</title></head>\n")
        request:send_data("<body><h1>", status, "</h1></body>\n")
        request:send_data("</html>\n")
      end
      if request.method == "GET" or request.method == "HEAD" then
        if request.path == "" then
          request:send_status("200 OK")
          request:send_header("Content-Type", "text/html; charset=UTF-8")
          request:send_data("<html>\n")
          request:send_data("<head><title>Hello World Application</title></head>\n")
          request:send_data("<body><h1>Hello World!</h1></body>\n")
          request:send_data("</html>\n")
        else
          error_response("404 Not Found")
        end
      else
        error_response("405 Method not allowed")
      end
      return true
    end
  )
}

