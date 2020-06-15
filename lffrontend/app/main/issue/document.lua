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

slot.put_into("toolbar", ui.tag{
  tag= "div",
  attr = { class="toolbar" },
  content=function()
    ui.tag{
      tag="div", attr={class="container"}, 
      content=function()
        ui.tag{
          tag="div", 
          attr={class="toolbar__title"},
          content=area.name,
        }
        ui.tag{tag="div", attr={class="toolbar__tab_container"},
          content=function()
            ui.tag{tag="div", attr={class="toolbar__tab"}, content=function()
              ui.link{module="issue", view="show", id=issue.id, content="Initiatives"}
            end}
            ui.tag{tag="div", attr={class="toolbar__tab toolbar__tab--active"},
              content="Document"}
          end,
        }
      end,
    }
  end,
})

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
    AreaID={"'" .. area.id .. "'"},
    UserID={tostring(app.session.member_id)},
    Op={"messages.CreateDocReq", "messages.GetDocReq"},
    Svc={"gitsvc"},
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
