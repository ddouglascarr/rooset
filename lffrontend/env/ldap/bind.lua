-- binds to configured LDAP server
-- --------------------------------------------------------------------------
-- omit arguments for anonymous bind
--
-- arguments:
--   dn: the distinguished name to be used fo binding (string)
--   password: password credentials (string)
--
-- returns:
--   ldap: in case of success, an LDAP connection handle
--   err: in case of an error, an error code (string)
--   err2: error dependent extra error information

function ldap.bind(dn, password)
  
  local libldap = require("mldap")

  local hostlist = ldap.get_hosts()

  -- try binding to LDAP server until success of no host entry left  
  local ldap
  while not ldap do
  
    if #hostlist < 1 then
      break
    end
    
    local host = table.remove(hostlist, 1)
    
    local err
    ldap, err, errno = libldap.bind{
      uri = host.uri,
      timeout = host.timeout,
      who = dn,
      password = password
    }
    
    if not err and ldap then
      return ldap, nil
    end

    local errno_string
    
    if errno then
      errno_string = libldap.errorcodes[errno]
    end
    
    if errno == libldap.errorcodes.invalid_credentials then
      return nil, "invalid_credentials", errno_string
    end
  end

  return nil, "cant_contact_ldap_server"
  
end
