local id = param.get_id()

local newsletter

if id then
  newsletter = Newsletter:by_id(id)
  if newsletter.sent then
    slot.select("error", function()
      ui.tag{ content = _"Newsletter has already been sent out" }
    end)
    return false
  end
else
  newsletter = Newsletter:new()
end

newsletter.published = param.get("published")
if newsletter.published == nil or newsletter.published == "" then
  newsletter.published = "now"
end
newsletter.unit_id = param.get("unit_id", atom.integer)
if newsletter.unit_id == 0 then
  newsletter.unit_id = nil
end
newsletter.include_all_members = param.get("include_all_members", atom.boolean)
newsletter.subject = param.get("subject")
newsletter.content = param.get("content")

newsletter:save()
