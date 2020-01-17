local issue
local area

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

ui.tag{ tag = "h3", content="Initiative Editor" }
ui.tag{ tag = "div", attr = { id="initiative-editor" }, content="" }
ui.tag{ tag = "div", attr = { id="initiative-form" }, content="" }
ui.tag{
  tag = "div",
  content = function()
    ui.field.hidden{ type="text", name="doc", value="", attr = { id="doc-field" } }
  end,
}

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
  Tk = jwt.encode({
    RepositoryName={area.unit.external_reference},
    AreaID={"'" .. area.id .. "'"},
    Op={"messages.NewInitiativeReq", "messages.GetDocReq"},
    Svc={"gitsvc"},
  })
}
if issue_id then
  new_initiative_payload['IssueID'] = issue_id
end

ui.tag{ tag="script", attr = { type="text/javascript" },  content=function() 
slot.put("/* <![CDATA[ */")
slot.put([[
  initNewInitiativePage(Object.assign({
    RootEl: document.getElementById('initiative-form'),
    ProseMirrorEl: document.getElementById('initiative-editor'),
  }, ]]  .. encode.json(new_initiative_payload) .. [[)
  );
]])
slot.put("/* ]]> */")
end
}

--   repositoryName: ']] .. area.unit.external_reference .. [[',
--   areaID: ']] .. area.id .. [[',
--   issueID: ']] .. issue_id == nil and '' or issue_id .. [[',
--   tk: ']] .. jwt.encode({
--   RepositoryName={area.unit.external_reference},
--   AreaID={"'" .. area.id .. "'"},
--   Op={"messages.NewInitiativeReq", "messages.GetDocReq"},
--   Svc={"gitsvc"},
-- }) .. "'})"
-- }
