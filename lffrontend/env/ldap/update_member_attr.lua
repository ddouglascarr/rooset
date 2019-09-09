-- check if the corresponding LDAP entry for an LiquidFeedback member
-- object is still existent and updates changed attributes
-- --------------------------------------------------------------------------
--
-- arguments:
--   member: a LiquidFeedback Member object (required)
--   ldap_conn: a ldap connection handle (optional)
--   uid: the uid of the member (optional, required when creating members)
--
-- returns:
--   ldap_conn: an LDAP connection
--   ldap_entry: the found LDAP entry (if any)
--   err: error code in case of an error (string)
--   err2: error dependent extra error information
--   err3: error dependent extra error information

function ldap.update_member_attr(member, ldap_conn, uid)
  
  -- do this only for members with ldap authentication
  if member.authority ~= "ldap" then
    return nil, nil, "member_is_not_authenticated_by_ldap"
  end
  
  local filter = config.ldap.member.uid_filter_map(member.authority_uid or uid)
  local ldap_entry, err, err2 = ldap.get_member_entry(filter, ldap_conn)

  if err then
    return ldap_conn, nil, "ldap_error", err, err2
  end
  
  -- If no corresponding entry found, lock the member
  if not ldap_entry then
    member.locked = true
    member.active = false
    return ldap_conn
  end

  -- If exactly one corresponding entry found, update the attributes
  local err = config.ldap.member.attr_map(ldap_entry, member)
  
  member.authority_uid = member.authority_uid or uid
  member.authority_login = config.ldap.member.login_map(ldap_entry)
  
  if err then
    return ldap_conn, ldap_entry, "attr_map_error", err
  end
  
  return ldap_conn, ldap_entry
    
end
