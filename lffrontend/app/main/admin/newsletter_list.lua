local newsletter = Newsletter:new_selector()
  :add_order_by("published DESC")
  :exec()

ui.titleAdmin(_"Newsletter")

ui.section( function()

  ui.sectionHead( function()
    ui.heading { level = 1, content = _"Newsletter list" }
  end )

  ui.sectionRow( function ()

    ui.list{
      records = newsletter,
      columns = {
        { label = _"Unit", content = function(r) ui.tag{ content = r.unit and r.unit.name or _"All members" } end },
        { name = "published", label = _"Published" },
        { name = "subject", label = _"Subject" },
        { label = _"sent", content = function(r) 
          if not r.sent then 
            ui.link{ text = _"Edit", module = "admin", view = "newsletter_edit", id = r.id } 
          else
            ui.tag{ content = format.timestamp(r.sent) }
          end 
        end }
      }
    }

  end)
end)
