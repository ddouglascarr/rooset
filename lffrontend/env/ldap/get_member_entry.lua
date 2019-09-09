-- gets the corresponding ldap entry for a given member login
-- --------------------------------------------------------------------------
--
-- arguments:
--   filter: the LDAP filter for searching the member (required)
--   use_ldap_conn: an already existing LDAP connection to be used (optional)
--
-- returns:
--   ldap_entry: in case of success, the LDAP entry (object)
--   err: in case of an error, an error message (string)
--   err2: error dependent extra error information

function ldap.get_member_entry(filter, use_ldap_conn)
  
  local ldap_conn, err
  
  if use_ldap_conn then
    ldap_conn = use_ldap_conn
  else
    ldap_conn, bind_err = ldap.bind_as_app()
  end
  
  if not ldap_conn then
    return nil, "ldap_bind_error", bind_err
  end

  local entries, search_err = ldap_conn:search{
    base = config.ldap.base,
    scope = config.ldap.member.scope,
    filter = filter,
    attr = config.ldap.member.fetch_attr,
  }
  
  if not use_ldap_conn then
    ldap_conn:unbind()
  end
  
  if not entries then
    return nil, "ldap_search_error", search_err
  end
  
  if #entries > 1 then
    return nil, "too_many_ldap_entries_found"
  end
  
  if #entries < 0 then
    return nil, "no_ldap_entry_found"
  end
  
  return entries[1]
  
end
