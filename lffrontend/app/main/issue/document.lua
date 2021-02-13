local issue
local area
slot.set_layout("rooset", "text/html")

local issue_id = param.get("issue_id", atom.integer)
if issue_id then
  issue = Issue:new_selector():add_where{"id=?",issue_id}:single_object_mode():exec()
  issue:load_everything_for_member_id(app.session.member_id)
  area = issue.area
else
  -- TODO: error
end

-- TODO(ddc): copy-pasted from document.lua<area>
local tab_link = function(is_active, content)
  local classname = "ml-2 flex-grow-0 text-xs "
  if is_active then
    classname = classname .. "text-black font-bold border-b-2 border-black cursor-default"
  else
    classname = classname .. "text-blue-600 hover:underline" 
  end
  
  ui.tag{
    tag="div",
    attr={class=classname},
    content=content,
  }
end

slot.put_into("toolbar", slot.use_temporary(function()
  ui.tag{
    tag= "div",
    attr = { class="max-w-xl mx-auto pt-2" },
    content=function()
      ui.tag{
        tag="h3", 
        attr={class="text-xl font-bold leading-tight text-grey-900"},
        content=area.name .. " › issue #" .. issue.id,
      }
      ui.tag{
        tag="div",
        attr={class="flex justify-end"},
        content=function()
          tab_link(false, function()
            ui.link{module="issue", view="show", id=issue.id, content="Initiatives"}
          end)
          tab_link(true, function()
              ui.tag{ content="Document" }
          end)
        end,
      }
    end,
  }
end))

ui.tag{ tag = "div", attr = { class="container" }, content=function()
  ui.tag{ tag = "div", attr = { id="react-root" }, content="" }
end}

ui.tag{
  tag = "script", 
  attr = {
    type="text/javascript",
    src=config.static_content_host_external .. "/competingInitiative.js",
  },
  content="",
}

local competing_initiative_payload = {
  DocsvcHostExternal=config.docsvc_host_external,
  AreaID = tostring(area.id),
  IssueID = tostring(issue_id),
  BaseDocSHA=area.current_external_reference,
  UserID=tostring(app.session.member_id),
  IssueSections=json.object{},  -- populated below
  Tk = jwt.encode({
    UserID=app.session.member_id,
    Op={"docsvc.CreateDocReq", "docsvc.GetDocReq"},
    SHA=area.current_external_reference,
    BaseSHA=area.current_external_reference,
  })
}

for k, section in ipairs(issue.sections)  do
  competing_initiative_payload['IssueSections'][section.external_reference] = json.object{}
end

ui.tag{ tag="script", attr = { type="text/javascript" },  content=function() 
  slot.put("/* <![CDATA[ */")
  slot.put([[
    initCompetingInitiativePage(
      document.getElementById('react-root'),
      ]]  .. encode.json(competing_initiative_payload) .. [[
    );
  ]])
  slot.put("/* ]]> */")
end
}
