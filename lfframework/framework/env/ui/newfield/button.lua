--[[--
ui.newfield.button{
  TODO
}

Button for a form

--]]--

function ui.newfield.button(args)
  local attr = args.attr 

  attr.class = "rst-btn"
  if args.mode and args.mode == "primary" then
    attr.class = attr.class .. " rst-btn--primary"
  end
  if attr.disabled then
    attr.class = attr.class .. " rst-btn--disabled"
  end
 
  ui.tag{
    tag = "button",
    attr = attr,
    content = args.content,
  }
end
