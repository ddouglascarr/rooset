IgnoredArea = mondelefant.new_class()
IgnoredArea.table = 'ignored_area'
IgnoredArea.primary_key = { "member_id", "area_id" }

function IgnoredArea:by_pk(member_id, area_id)
  return self:new_selector()
    :add_where{ "member_id = ?", member_id }
    :add_where{ "area_id = ?", area_id }
    :optional_object_mode()
    :exec()
end

function IgnoredArea:destroy_by_member_id(member_id)
  local ignored_areas = self:new_selector()
    :add_where{ "member_id = ?", member_id }
    :exec()
  for i, ignored_area in ipairs(ignored_areas) do
    ignored_area:destroy()
  end
end