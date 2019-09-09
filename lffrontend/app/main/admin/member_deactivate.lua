local id = param.get_id()

local member = Member:by_id(id)

ui.title(_"Deactivate member")


ui.form{
  attr = { class = "vertical section" },
  module = "admin",
  action = "member_deactivate",
  id = member and member.id,
  record = member,
  routing = {
    error = {
      mode = "forward",
      module = "admin", view = "member_deactivate", id = id
    },
    default = {
      mode = "redirect",
      modules = "admin", view = "index"
    }
  },
  content = function()
    ui.sectionHead( function()
      ui.heading { level = 1, content = member and (member.name or member.id) or _"New member" }
      if member and member.identification then
        ui.heading { level = 3, content = member.identification }
      end
    end )
    ui.sectionRow(function()
      ui.heading { level = 2, content = _"Do you really want to irrevocably deactive this member?" }
      ui.tag{ tag = "input", attr = { type = "checkbox", name = "sure", value = "yes" } }
      ui.tag { content = _"I want to deactive this member irrevocably" }
      slot.put("<br />")
      slot.put("<br />")
      ui.submit{ text = _"Deactivate member" }
      slot.put(" ")
      ui.link { module = "admin", view = "member_edit", id = member.id, content = _"cancel" }
    end)
  end
}

