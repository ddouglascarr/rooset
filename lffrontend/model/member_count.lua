MemberCount = mondelefant.new_class()
MemberCount.table = 'member_count'

function MemberCount:get()
  if not MemberCount.total_count then
    local member_count = self:new_selector():optional_object_mode():exec()
    if member_count then
      MemberCount.total_count = member_count.total_count
    end
  end
  return MemberCount.total_count
end