-- binds to configured LDAP server with application's credentials
-- --------------------------------------------------------------------------
--
-- returns
--   ldap_conn: in case of success, an LDAP connection handle
--   err: in case of an error, an error code (string)

function ldap.bind_as_app()

  local dn, password
  
  if config.ldap.bind_as then
    dn       = config.ldap.bind_as.dn
    password = config.ldap.bind_as.password
  end
  
  local ldap_conn, err = ldap.bind(dn, password)
  
  return ldap_conn, err
  
end
