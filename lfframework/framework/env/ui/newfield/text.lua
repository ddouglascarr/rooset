--[[--
ui.newfield.text{
  TODO
}

Text field with label

--]]--

function ui.newfield.text(args)
  local value_string = format.string(args.value, args.format_options)
  local attr = table.new(args.attr)
  attr.name  = args.html_name

  local label_attr = table.new(attr)
  label_attr.class = 'rst-label'
  attr.class = 'rst-text-input'
  attr.type = attr.type or 'text'

  ui.tag{
    tag = 'div',
    attr = { class = 'space-y' },
    content = function()
      ui.tag{
        tag = 'label',
        attr = label_attr,
        content=args.label,
      }
      ui.tag{
        tag = 'input',
        attr = attr,
      }
    end,
  }
end
