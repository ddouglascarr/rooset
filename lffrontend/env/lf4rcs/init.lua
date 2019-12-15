function lf4rcs.init()
  local super_handler = config.notification_handler_func
  config.notification_handler_func = function(event)
    if super_handler then super_handler(event) end
    lf4rcs.notification_handler(event)
  end
  config.render_external_reference = {
    draft = lf4rcs.render_draft_reference,
    initiative = lf4rcs.render_initiative_reference
  }
end

