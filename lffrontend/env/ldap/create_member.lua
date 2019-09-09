-- Create a new member object from LDAP for an uid
-- --------------------------------------------------------------------------
--
-- arguments:
--   uid: uid of the new member object (required)
--
-- returns:
--   member: a LiquidFeedback Member object (in case of success)
--   err: error code in case of an error (string)
--   err2: error dependent extra error information

function ldap.create_member(uid)
  
  local member = Member:new()

  member.authority = "ldap"

  member.authority_uid = uid
  
  local ldap_conn, ldap_entry, err, err2 = ldap.update_member_attr(member, nil, uid)
  
  if ldap_conn then
    ldap_conn:unbind()
  end
  
  member.authority_uid = uid
  member.authority_login = config.ldap.member.login_map(ldap_entry)
  
  if not err then
    return member
  end
  
  return nil, err, err2
  
end
