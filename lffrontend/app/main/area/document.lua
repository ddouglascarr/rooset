local issue
local area
slot.set_layout("rooset", "text/html")

local issue_id = param.get("issue_id", atom.integer)
if issue_id then
  issue = Issue:new_selector():add_where{"id=?",issue_id}:single_object_mode():exec()
  issue:load_everything_for_member_id(app.session.member_id)
  area = issue.area
else
  local area_id = param.get("area_id", atom.integer)
  area = Area:new_selector():add_where{"id=?",area_id}:single_object_mode():exec()
  area:load_delegation_info_once_for_member_id(app.session.member_id)
end

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

-- load the current document

slot.put_into("toolbar", slot.use_temporary(function()
  ui.tag{
    tag= "div",
    attr = { class="max-w-xl mx-auto pt-2" },
    content=function()
      ui.tag{
        tag="h3", 
        attr={class="text-xl font-bold leading-tight text-grey-900"},
        content=area.name,
      }
      ui.tag{
        tag="div",
        attr={class="flex justify-end"},
        content=function()
          -- ui.tag{tag="div", attr={class="flex-grow"}, content=""}
          
          tab_link(true, function()
            ui.tag{
              content="Document",
            }
          end)
          tab_link(false, function()
            ui.link{
              module="area",
              view="show",
              id=area.id,
              content="Proposals",
            }
          end)
        end,
      }
    end,
  }
end))

ui.tag{ tag = "div", attr = { class="container" }, content=function()
  ui.tag{ tag = "div", attr = { id="initiative-form" }, content="" }
end}

ui.tag{
  tag = "script", 
  attr = {
    type="text/javascript",
    src=config.static_content_host_external .. "/newInitiative.js",
  },
  content="",
}

local new_initiative_payload = {
  RepositoryName = area.unit.external_reference,
  DocsvcHostExternal=config.docsvc_host_external,
  AreaID = tostring(area.id),
  DocSHA=area.current_external_reference,
  UserID=tostring(app.session.member_id),
  OpenAdmittedSections=json.object{},  -- populated below
  Tk = jwt.encode({
    RepositoryName={area.unit.external_reference},
    AreaID={"'" .. area.id .. "'"},
    UserID={tostring(app.session.member_id)},
    Op={"messages.CreateDocReq", "messages.GetDocReq"},
    Svc={"gitsvc"},
  })
}

if issue_id then
  new_initiative_payload['IssueID'] = issue_id
end

for k, open_section in ipairs(area.open_admitted_sections)  do
  new_initiative_payload['OpenAdmittedSections'][open_section.external_reference] = {
    IssueID = open_section.issue_id,
  }
end

ui.tag{ tag="script", attr = { type="text/javascript" },  content=function() 
slot.put("/* <![CDATA[ */")
slot.put([[
  initNewInitiativePage(Object.assign({
    RootEl: document.getElementById('initiative-form'),
  }, ]]  .. encode.json(new_initiative_payload) .. [[)
  );
]])
slot.put("/* ]]> */")
end
}
