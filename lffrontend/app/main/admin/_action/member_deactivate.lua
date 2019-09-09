local sure = param.get("sure")

if sure ~= "yes" then
  slot.select("error", function()
    ui.tag{ content = _"You need to confirm to deactivate!" }
  end)
  return false
end

local id = param.get_id()
local clean = param.get("clean")

local member = Member:by_id(id)

member:delete()

slot.select("notice", function()
  ui.tag{ content = _"Member successfully deactivated" }
end )
