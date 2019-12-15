function ui.filters(args)
  local el_id = ui.create_unique_id()
  ui.container{
    attr = { class = "ui_filter" },
    content = function()
      for idx, filter in ipairs(args) do
        local filter_name = filter.name or "filter"
        local current_option = atom.string:load(request.get_param{ name = filter_name })
        if not current_option then
          current_option = param.get(filter_name)
        end
        local current_option_valid = false
        for idx, option in ipairs(filter) do
          if current_option == option.name then
            current_option_valid = true
          end
        end
        if not current_option or #current_option == 0 or not current_option_valid then
          current_option = filter[1].name
        end
        local id     = request.get_id_string()
        local params = request.get_param_strings()
        local class = "ui_filter_head"
        if filter.class then
          class = class .. " " .. filter.class
        end
        ui.container{
          attr = { class = class },
          content = function()
            slot.put(filter.label)
            for idx, option in ipairs(filter) do
              params[filter_name] = option.name
              local attr = {}
              if current_option == option.name then
                attr.class = "active"
                option.selector_modifier(args.selector)
              end
              if idx > 1 then
                slot.put(" ")
              end
              ui.link{
                attr    = attr,
                module  = request.get_module(),
                view    = request.get_view(),
                id      = id,
                params  = params,
                text    = option.label,
                partial = {
                  params = {
                    [filter_name] = option.name
                  }
                }
              }
            end
          end
        }
      end
    end
  }
  ui.container{
    attr = { class = "ui_filter_content" },
    content = function()
      args.content()
    end
  }
end
