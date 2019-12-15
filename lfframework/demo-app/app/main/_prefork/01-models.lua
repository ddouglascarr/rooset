function mondelefant.class_prototype:by_id(id)
  return self:new_selector()
    :add_where{ "id = ?", id }
    :optional_object_mode()
    :exec()
end
execute.inner()
