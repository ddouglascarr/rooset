-- Update member privileges from LDAP
-- --------------------------------------------------------------------------
--
-- arguments:
--   member: the member for which the privileges should be updated
--   ldap_entry: the ldap entry to be used for updating the privileges
--
-- returns:
--   err: an error code, if an error occured (string)
--   err2: Error dependent extra error information

function ldap.update_member_privileges(member, ldap_entry)

  local privileges, err = config.ldap.member.privilege_map(ldap_entry, member)

  if err then
    return false, "privilege_map_error", err
  end

  local privileges_by_unit_id = {}
  for i, privilege in ipairs(privileges) do
    privileges_by_unit_id[privilege.unit_id] = privilege
  end

  local current_privileges = Privilege:by_member_id(member.id)
  local current_privilege_ids = {}

  for i, privilege in ipairs(current_privileges) do
    if privileges_by_unit_id[privilege.unit_id] then
      current_privilege_ids[privilege.unit_id] = privilege
    else
      privilege:destroy()
    end
  end

  for i, privilege in ipairs(privileges) do
    local current_privilege = current_privilege_ids[privilege.unit_id]
    if not current_privilege then
      current_privilege = Privilege:new()
      current_privilege.member_id = member.id
      current_privileges[#current_privileges+1] = current_privilege
    end
    for key, val in pairs(privilege) do
      current_privilege[key] = val
    end
  end

  for i, privilege in ipairs(current_privileges) do
    local err = privilege:try_save()
    if err then
      return false, "privilege_save_error", err
    end
  end

  return true
end
