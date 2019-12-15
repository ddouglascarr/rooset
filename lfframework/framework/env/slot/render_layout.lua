--[[--
output =              -- document/data to be sent to the web browser
slot.render_layout()

This function returns the selected layout after replacing all slot placeholders with the respective slot contents. If slot.set_layout(...) was called with nil as first argument, then no layout will be used, but only the contents of the slot named "data" are returned.

--]]--

function slot.render_layout()
  if slot._current_layout then
    local layout_file = assert(io.open(
      encode.file_path(
        WEBMCP_BASE_PATH,
        'app',
        WEBMCP_APP_NAME,
        '_layout',
        slot._current_layout .. '.html'
      ),
      'r'
    ))
    local layout = assert(layout_file:read("*a"))
    assert(layout_file:close())

    -- render layout
    layout = string.gsub(layout, "__BASEURL__/?", request.get_relative_baseurl())  -- TODO: find a better placeholder than __BASEURL__ ?
    layout = string.gsub(layout, '<!%-%- *WEBMCP +SLOT +([^ ]+) *%-%->', 
      function(slot_ident)
        if #slot.get_content(slot_ident) > 0 then
          return '<div class="slot_' .. slot_ident .. '" id="slot_' .. slot_ident .. '">' .. slot.get_content(slot_ident).. '</div>'
        else
          return ''
        end
      end
    )
    layout = string.gsub(layout, '<!%-%- *WEBMCP +SLOTNODIV +([^ ]+) *%-%->', 
      function(slot_ident)
        if #slot.get_content(slot_ident) > 0 then
          return slot.get_content(slot_ident)
        else
          return ''
        end
      end
    )
    return layout
  else
    return slot.get_content("data")
  end
end
