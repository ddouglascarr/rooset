local id = param.get_id()

local member = Member:by_id(id)

local deactivated = member and member.locked and member.login == nil and member.authority_login == nil

ui.titleAdmin(_"Member")

local units_selector = Unit:new_selector()
  
if member then
  units_selector
    :left_join("privilege", nil, { "privilege.member_id = ? AND privilege.unit_id = unit.id", member.id })
    :add_field("privilege.voting_right", "voting_right")
end

local units = units_selector:exec()
  
ui.form{
  attr = { class = "vertical section" },
  module = "admin",
  action = "member_update",
  id = member and member.id,
  record = member,
  readonly = not app.session.member.admin,
  routing = {
    default = {
      mode = "redirect",
      modules = "admin",
      view = "index"
    }
  },
  content = function()

    ui.sectionHead( function()
      ui.heading { level = 1, content = member and (member.name or member.id) or _"New member" }
      if member and member.identification then
        ui.heading { level = 3, content = member.identification }
      end
    end )
  
    ui.sectionRow( function()
      ui.field.text{     label = _"Identification", name = "identification" }
      ui.field.text{     label = _"Notification email", name = "notify_email" }
      if member and member.activated then
        ui.field.text{     label = _"Screen name",        name = "name" }
      end
      
      if member and member.activated and not deactivated then
        ui.field.text{     label = _"Login name",        name = "login" }
      end

      for i, unit in ipairs(units) do
        ui.field.boolean{
          name = "unit_" .. unit.id,
          label = unit.name,
          value = unit.voting_right
        }
      end
      slot.put("<br /><br />")

      if member then
        ui.field.text{  label = _"Activated",       name = "activated", readonly = true }
      end
         
      if not member or not member.activated then
        ui.field.boolean{  label = _"Send invite?",       name = "invite_member" }
      end
      
      if member then
        ui.field.boolean{ 
          label = _"Member inactive?", name = "deactivate",
          readonly = true, 
          value = member and member.active == false
        }
      end
      
      if member then
        ui.field.boolean{
          label = _"Lock member?", name = "locked",
        }
      end
      
      slot.put("<br />")
      ui.field.boolean{  label = _"Admin?", name = "admin" }
      slot.put("<br />")
      ui.submit{         text  = _"update member" }
      slot.put(" ")
      if member then
        ui.link { module = "admin", view = "member_deactivate", content = _"Deactivate member", id = member.id }
        slot.put(" ")
      end
      ui.link { module = "admin", view = "index", content = _"cancel" }
    end )
  end
}

