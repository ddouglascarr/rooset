-- check if credentials (given by a user) are valid to bind to LDAP
-- --------------------------------------------------------------------------
--
-- arguments:
--   dn: The distinguished name to be used fo binding (string, required)
--   password: Password credentials (string, required)
--
-- returns
--   success: true in cases of valid credentials
--            false in cases of invalid credentials
--            nil in undetermined cases, i.e. unavailable LDAP server
--   err: error code in case of errors, otherwise nil (string)
--   err2: error dependent extra error information

function ldap.check_credentials(login, password)

  local filter = config.ldap.member.login_filter_map(login)
  local ldap_entry, err, err2 = ldap.get_member_entry(filter)

  if err == "too_many_entries_found" then
    return false, "invalid_credentials"
  end
  
  if err then
    return nil, err
  end
  if not ldap_entry then
    return false, "invalid_credentials"
  end
  
  local dn = ldap_entry.dn
  
  local ldap, err, err2 = ldap.bind(dn, password)
 
  if err == "invalid_credentials" then
    return false, "invalid_credentials"
  end
  
  if err then
    return nil, err, err2
  end
  
  ldap:unbind()
  
  return ldap_entry
  
end
