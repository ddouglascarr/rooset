-- check for all LiquidFeedback Members with LDAP authentication
-- if the corresponding LDAP entry is still existent and updates
-- changed attributes
-- --------------------------------------------------------------------------
-- prints debug output to stdout
--
-- returns
--   success: true if no error occured during run
--            false if at least one error occured during run

function ldap.update_all_members()

  local some_error_occured = false
  
  local ldap_conn = ldap.bind_as_app()
  
  function update_member(member)

    local function failure (err, err2)
      Member.get_db_conn():query("ROLLBACK")
      io.stdout:write("ERROR: ", err, " (", err2, ") id=", tostring(member.id), " uid=", tostring(member.authority_uid), "\n")
      some_error_occured = true
    end
    
    local function success ()
      Member.get_db_conn():query("COMMIT")
      io.stdout:write("ok: id=", tostring(member.id), " uid=", tostring(member.authority_uid), "\n")
    end

    Member.get_db_conn():query("BEGIN")

    local ldap_conn, ldap_entry, err, err2 = ldap.update_member_attr(member, ldap_conn)
    if err then
      failure("ldap_update_member", err)
      return
    end

    local err = member:try_save()
    if err then
      failure("member_try_save", err)
      return
    end

    if ldap_entry then
      local success, err, err2 = ldap.update_member_privileges(member, ldap_entry)
      if err then
        failure("ldap_update_member_privileges", err)
        return
      end
      
    end

    success()

  end

  
  local members = Member:get_all_by_authority("ldap")
    
  for i, member in ipairs(members) do 
    update_member(member)
  end
  
  ldap_conn:unbind()
  
  return not some_error_occured
  
end
