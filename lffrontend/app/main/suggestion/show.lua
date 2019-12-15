local suggestion = Suggestion:by_id(param.get_id())

-- redirect to initiative if suggestion does not exist anymore
if not suggestion then
  local initiative_id = param.get('initiative_id', atom.integer)
  if initiative_id then
    slot.reset_all{except={"notice", "error"}}
    request.redirect{
      module='initiative',
      view='show',
      id=initiative_id,
      params = { tab = "suggestions" }
    }
  else
    slot.put_into('error', _"Suggestion does not exist anymore")
  end
  return
end

local initiative = suggestion.initiative

initiative:load_everything_for_member_id(app.session.member_id)
initiative.issue:load_everything_for_member_id(app.session.member_id)


execute.view{ module = "issue", view = "_sidebar_state", params = {
  initiative = initiative
} }

execute.view { 
  module = "issue", view = "_sidebar_issue", 
  params = {
    issue = initiative.issue,
    highlight_initiative_id = initiative.id
  }
}

execute.view {
  module = "issue", view = "_sidebar_whatcanido",
  params = { initiative = initiative }
}

execute.view { 
  module = "issue", view = "_sidebar_members", params = {
    issue = initiative.issue, initiative = initiative
  }
}



execute.view {
  module = "issue", view = "_head", params = {
    issue = initiative.issue
  }
}


ui.section( function()
  ui.sectionHead( function()
    ui.link{
      module = "initiative", view = "show", id = initiative.id,
      content = function ()
        ui.heading { 
          level = 1,
          content = initiative.display_name
        }
      end
    }
    ui.heading { level = 2, content = _("Suggestion for improvement #{id}", { id = suggestion.id }) }
  end )
  ui.sectionRow( function()

    ui.heading{ level = 2, content = suggestion.name }
    if app.session:has_access("authors_pseudonymous") and suggestion.author then 
      util.micro_avatar(suggestion.author)
    end
  end )
  ui.sectionRow( function()
    ui.container{
      attr = { class = "suggestion_content wiki" },
      content = function()
        slot.put(suggestion:get_content("html"))
      end
    }
    
  end )
end )

ui.section( function()
  ui.sectionHead( function()
    ui.heading { level = 2, content = _"Collective rating" }
  end )
  ui.sectionRow( function()

    execute.view{
      module = "suggestion",
      view = "_list_element",
      params = {
        suggestions_selector = Suggestion:new_selector():add_where{ "id = ?", suggestion.id },
        initiative = suggestion.initiative,
        show_name = false,
        show_filter = false
      }
    }
  end)
end)
  
if app.session:has_access("all_pseudonymous") then
  ui.section( function()
    ui.sectionHead( function()
      ui.heading { level = 2, content = _"Individual ratings" }
    end )
    ui.sectionRow( function()

      execute.view{
        module = "opinion",
        view = "_list",
        params = { 
          opinions_selector = Opinion:new_selector()
            :add_where{ "suggestion_id = ?", suggestion.id }
            :join("member", nil, "member.id = opinion.member_id")
            :add_order_by("member.id DESC")
        }
      }

    end)
  end)
end