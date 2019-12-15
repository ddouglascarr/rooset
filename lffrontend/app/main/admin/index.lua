local inactive = param.get("inactive", atom.boolean)

local units = Unit:get_flattened_tree{ include_inactive = inactive }

local policies = Policy:build_selector{}:add_order_by("index"):exec()

ui.titleAdmin()

ui.sidebar( "tab-members", function()
  ui.sidebarHead( function()
    ui.heading { level = 2, content = _"Members" }
  end )
  
  ui.sidebarSection( function()
    ui.tag { tag = "ul", attr = { class = "ul" }, content = function()
      ui.tag { tag = "li", content = function()
        ui.form{
          module = "admin", view = "member_list",
          content = function()
          
            ui.field.text{ label = _"search", name = "search" }
            
            ui.submit{ value = _"search" }
          
          end
        }
      end }
    end }
  end )
  ui.sidebarSection( "moreLink", function()
    ui.link{
      text = _"Register new member",
      module = "admin",
      view = "member_edit"
    }
  end )
end )

ui.sidebar( "tab-whatcanido", function()
  ui.sidebarHead( function()
    ui.heading { level = 2, content = _"Newsletter" }
  end )
  
  ui.sidebarSection( "moreLink", function()
    ui.link{
      text = _"Create a newsletter",
      module = "admin",
      view = "newsletter_edit"
    }
  end )
  ui.sidebarSection( "moreLink", function()
    ui.link{
      text = _"Manage newsletters",
      module = "admin",
      view = "newsletter_list"
    }
  end )
end )

ui.sidebar( "tab-whatcanido", function()
  ui.sidebarHead( function()
    ui.heading { level = 2, content = _"Cancel issue" }
  end )
  
  ui.sidebarSection( function()
    ui.form{
      module = "admin",
      view = "cancel_issue",
      content = function()
        ui.tag { tag = "ul", attr = { class = "ul" }, content = function()
          ui.tag { tag = "li", content = function()
            ui.field.text{ label = _"Issue #", name = "id" }
            ui.submit{ text = _"cancel issue" }
          end }
        end }
      end
    }
  end )
end )

ui.sidebar("tab-whatcanido", function()
  ui.sidebarHead( function()
    ui.heading { level = 2, content = _"Policies" }
  end )
  
  ui.sidebarSection( function()
    ui.tag { tag = "ul", attr = { class = "ul" }, content = function()
      for i, policy in ipairs(policies) do
        ui.tag { tag = "li", content = function()
          ui.link{
            content = policy.name,
            module = "admin",
            view = "policy_show",
            id = policy.id
          }
        end }
      end
    end }
  end )
  ui.sidebarSection( "moreLink", function()
    ui.link{
      text = _"Create new policy",
      module = "admin",
      view = "policy_show"
    }
  end )
  ui.sidebarSection( "moreLink", function()
    ui.link{
      text = _"Show policies not in use",
      module = "admin",
      view = "policy_list",
      params = { show_not_in_use = true }
    }
  end )
end )


ui.section( function()
  ui.sectionHead( function()
    ui.heading { level = 1, content = _"Organizational units and subject areas" }
  end )
  ui.sectionRow( function()

    for i_unit, unit in ipairs(units) do
      ui.container { 
        attr = { style = "margin-left: " .. ((unit.depth - 1)* 2) .. "em;" },
        content = function ()
          ui.heading { level = 1, content = function ()
            local class
            if unit.active == false then
              class = "inactive"
            end
            ui.link{ attr = { class = class }, text = unit.name, module = "admin", view = "unit_edit", id = unit.id }
          end }
          ui.tag { tag = "ul", attr = { class = "ul" }, content = function ()
            local areas
            if not inactive then
              areas = unit:get_reference_selector("areas"):add_order_by("name"):add_where("active"):exec()
            else
              areas = unit:get_reference_selector("areas"):add_order_by("name"):exec()
            end
            for i, area in ipairs(areas) do
              ui.tag { tag = "li", content = function ()
                local class
                if area.active == false then
                  class = "inactive"
                end
                ui.link{ attr = { class = class }, text = area.name, module = "admin", view = "area_show", id = area.id }
              end }
            end
            ui.tag { tag = "li", content = function ()
              ui.link { module = "admin", view = "area_show", params = { unit_id = unit.id }, content = _"+ add new subject area" }
            end }
            slot.put("<br />")
          end }
        end
      }
    end
    
    slot.put("<br />")
    ui.link { module = "admin", view = "unit_edit", content = _"Create new unit" }
    slot.put("<br />")
    slot.put("<br />")
    
    if (not inactive) then
      ui.link { module = "admin", view = "index", params = { inactive = true }, content = _"Show inactive" }
    else
      ui.link { module = "admin", view = "index", content = _"Hide inactive" }
    end
      
  end)
end)
