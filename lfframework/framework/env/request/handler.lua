--[[--
success =         -- false if an error occurred, true otherwise
request.handler(
  http_request    -- HTTP request object
)

Called by mcp.lua to process an HTTP request. Calls request.router() and handles the request. Note: request initializers (see request.initialize()) are to be executed by mcp.lua before this function is invoked by mcp.lua.

--]]--

function request.handler(http_request)
  request._http_request = http_request
  local path = http_request.path
  if path then
    local relative_baseurl_elements = {}
    for match in string.gmatch(path, "/") do
      relative_baseurl_elements[#relative_baseurl_elements+1] = "../"
    end
    if #relative_baseurl_elements > 0 then
      request._relative_baseurl = table.concat(relative_baseurl_elements)
    else
      request._relative_baseurl = "./"
    end
  else
    request._relative_baseurl = nil
  end
  request._route = request.router()
  do
    local post_id = http_request.post_params["_webmcp_id"]
    if post_id then
      request._route.id = post_id
    end
  end

  local success, error_info = xpcall(
    function()

      if not request._route then
        request._route = {}
        if request.get_404_route() then
          request.set_status("404 Not Found")
          request.forward(request.get_404_route())
        else
          error("Could not route request URL")
        end
      end

      if request._route.static then
        local subpath = request._route.static
        for element in string.gmatch(subpath, "[^/]+") do
          if element == "." or element == ".." then
            subpath = nil
            break
          end
        end
        local fstat, f, errmsg
        if subpath then
          local filename = WEBMCP_BASE_PATH .. "static/" .. subpath
          fstat, errmsg = extos.stat(filename)
          if fstat then
            if fstat.isdir then
              errmsg = "Is a directory"
            elseif not fstat.isreg then
              errmsg = "Not a regular file"
            else
              f, errmsg = io.open(filename, "r")
            end
          end
        end
        if not f then
          if request.get_404_route() then
            request.set_status("404 Not Found")
            request.forward(request.get_404_route())
          else
            error('Could not open static file "' .. subpath .. '": ' .. errmsg)
          end
        else
          local d = assert(f:read("*a"))
          f:close()
          slot.put_into("data", d)
          local filename_extension = string.match(subpath, "%.([^.]+)$")
          slot.set_layout(nil, request._mime_types[filename_extension] or "application/octet-stream")
          request.allow_caching()
          return
        end
      end

      -- restore slots if coming from http redirect
      do
        local tempstore_value = http_request.get_params["_tempstore"]
        if tempstore_value then
          trace.restore_slots{}
          local blob = tempstore.pop(tempstore_value)
          if blob then slot.restore_all(blob) end
        end
      end

      if request.get_action() then
        trace.request{
          module = request.get_module(),
          action = request.get_action()
        }
        if
          not execute.action{
            module = request.get_module(),
            action = request.get_action(),
            test_existence = true
          } and
          request.get_404_route()
        then
          request.set_status("404 Not Found")
          request.forward(request.get_404_route())
        else
          if http_request.method ~= "POST" then
            request.set_status("405 Method Not Allowed")
            request.add_header("Allow", "POST")
            error("Tried to invoke an action with a GET request.")
          end
          local action_status = execute.filtered_action{
            module = request.get_module(),
            action = request.get_action(),
          }
          if not request.is_rerouted() then
            local routing_mode, routing_module, routing_view, routing_anchor
            routing_mode   = http_request.post_params["_webmcp_routing." .. action_status .. ".mode"]
            routing_module = http_request.post_params["_webmcp_routing." .. action_status .. ".module"]
            routing_view   = http_request.post_params["_webmcp_routing." .. action_status .. ".view"]
            routing_anchor = http_request.post_params["_webmcp_routing." .. action_status .. ".anchor"]
            if not (routing_mode or routing_module or routing_view) then
              action_status = "default"
              routing_mode   = http_request.post_params["_webmcp_routing.default.mode"]
              routing_module = http_request.post_params["_webmcp_routing.default.module"]
              routing_view   = http_request.post_params["_webmcp_routing.default.view"]
              routing_anchor = http_request.post_params["_webmcp_routing.default.anchor"]
            end
            assert(routing_module, "Routing information has no module.")
            assert(routing_view,   "Routing information has no view.")
            if routing_mode == "redirect" then
              local routing_params = {}
              for key, value in pairs(request.get_param_strings{ method="POST", include_internal=true }) do
                local status, stripped_key = string.match(
                  key, "^_webmcp_routing%.([^%.]*)%.params%.(.*)$"
                )
                if status == action_status then
                  routing_params[stripped_key] = value
                end
              end
              request.redirect{
                module = routing_module,
                view   = routing_view,
                id     = http_request.post_params["_webmcp_routing." .. action_status .. ".id"],
                params = routing_params,
                anchor = routing_anchor
              }
            elseif routing_mode == "forward" then
              request.forward{ module = routing_module, view = routing_view }
            else
              error("Missing or unknown routing mode in request parameters.")
            end
          end
        end
      else
        -- no action
        trace.request{
          module = request.get_module(),
          view   = request.get_view()
        }
        if
          not execute.view{
            module = request.get_module(),
            view   = request.get_view(),
            test_existence = true
          } and request.get_404_route()
        then
          request.set_status("404 Not Found")
          request.forward(request.get_404_route())
        end
      end

      if not request.get_redirect_data() then
        request.process_forward()
        local view = request.get_view()
        if string.find(view, "^_") then
          error("Tried to call a private view (prefixed with underscore).")
        end
        execute.filtered_view{
          module = request.get_module(),
          view   = view,
        }
      end

    end,

    function(errobj)
      return {
        errobj = errobj,
        stacktrace = string.gsub(
          debug.traceback('', 2),
          "^\r?\n?stack traceback:\r?\n?", ""
        )
      }
    end
  )

  if not success then trace.error{} end

  slot.select('trace', trace.render)  -- render trace information

  local redirect_data = request.get_redirect_data()

  -- log error and switch to error layout, unless success
  if not success then
    local errobj     = error_info.errobj
    local stacktrace = error_info.stacktrace
    if not request._status then
      request._status = "500 Internal Server Error"
    end
    http_request:close_after_finish()
    slot.set_layout('system_error')
    slot.select('system_error', function()
      if getmetatable(errobj) == mondelefant.errorobject_metatable then
        slot.put(
          "<p>Database error of class <b>",
          encode.html(errobj.code),
          "</b> occured:<br/><b>",
          encode.html(errobj.message),
          "</b></p>"
        )
      else
        slot.put("<p><b>", encode.html(tostring(errobj)), "</b></p>")
      end
      slot.put("<p>Stack trace follows:<br/>")
      slot.put(encode.html_newlines(encode.html(stacktrace)))
      slot.put("</p>")
    end)
  elseif redirect_data then
    redirect_data = table.new(redirect_data)
    redirect_data.params = table.new(redirect_data.params)
    local slot_dump = slot.dump_all()
    if slot_dump ~= "" then
      redirect_data.params._tempstore = tempstore.save(slot_dump)
    end
    http_request:send_status("303 See Other")
    for i, header in ipairs(request._response_headers) do
      http_request:send_header(header[1], header[2])
    end
    http_request:send_header("Location", encode.url(redirect_data))
    http_request:finish()
  end

  if not success or not redirect_data then

    http_request:send_status(request._status or "200 OK")
    for i, header in ipairs(request._response_headers) do
      http_request:send_header(header[1], header[2])
    end
    if not request._cache_manual then
      local cache_time = request._cache_time
      if request._cache and cache_time and cache_time > 0 then
        http_request:send_header("Cache-Control", "max-age=" .. cache_time)
      else
        http_request:send_header("Cache-Control", "no-cache")
      end
    end

    -- TODO(ddc): handle request headers with multiple accept values
    if http_request.headers['Accept'][1] == 'application/json' then
      http_request:send_header("Content-Type", "application/json")
      http_request:send_data(encode.json(request.data))
    else
      http_request:send_header("Content-Type", slot.get_content_type())
      http_request:send_data(slot.render_layout())
    end
    http_request:finish()
  end

  return success

end
