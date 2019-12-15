function ui.titleAdmin(title)
  ui.title(function()
    if title then
      ui.link { module = "admin", view = "index", content = _"System administration" }
    else
      ui.tag{ content = _"System administration" }
    end
    if title then
      slot.put ( " » " )
      ui.tag { tag = "span", content = title }
    end
  end)
end