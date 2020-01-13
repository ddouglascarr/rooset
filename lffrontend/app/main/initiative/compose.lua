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

ui.tag{ tag = "script", attr = { type="text/javascript", src="http://localhost:8082/newInitiative.js" }, content="" }
ui.tag{ tag = "script", attr = { type="text/javascript" }, content=[[
initNewInitiativePage({
  rootEl: document.getElementById('initiative-form'),
  proseMirrorEl: document.getElementById('initiative-editor'),
  repositoryName: ']] .. area.unit.external_reference .. [[',
  areaID: ']] .. area.id .. [[',
  tk: ']] .. jwt.encode({
  RepositoryName={area.unit.external_reference},
  AreaID={"'" .. area.id .. "'"},
  Op={"messages.NewInitiativeReq", "messages.GetDocReq"},
  Svc={"gitsvc"},
}) .. "'})"
}
