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
  label_attr.class = 'text-sm font-semibold'
  attr.class = 'appearance-none relative block w-full px-3 py-2 rounded-md border border-gray-300 text-gray-900 focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm'
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
