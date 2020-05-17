function ui.icon( icon_name )
  local icon_file = assert(io.open(
    encode.file_path(
      WEBMCP_BASE_PATH,
      'app',
      WEBMCP_APP_NAME,
      '_icon',
      icon_name .. '.svg'
    ),
    'r'
  ))
  local icon = assert(icon_file:read("*a"))
  assert(icon_file:close())

  ui.tag {
    tag = 'div',
    attr = { class = 'icon24' },
    content = function() 
      slot.put(icon)
    end,
  }
end
