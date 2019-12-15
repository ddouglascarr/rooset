local area_id = assert(param.get("area_id", atom.integer), "no area id given")


local ignored_area = IgnoredArea:by_pk(app.session.member_id, area_id)

if param.get("delete", atom.boolean) then
  if ignored_area then
    ignored_area:destroy()
    slot.select("notice", function() ui.tag{ content = _"You have been subscribed for update emails about this subject area" } end)
  end
  return
end

if not ignored_area then
  local ignored_area = IgnoredArea:new()
  ignored_area.member_id = app.session.member_id
  ignored_area.area_id = area_id
  ignored_area:save()
  slot.select("notice", function() ui.tag{ content = _"You will no longer receive update emails about this subject area" }  end )

end
