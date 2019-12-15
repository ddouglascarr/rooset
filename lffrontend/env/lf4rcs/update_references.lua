function lf4rcs.update_references(repository, path, unit_id)
  local function log(message)
    print(lf4rcs.log_prefix .. message)
  end
  if not config.lf4rcs[repository]
      or not config.lf4rcs[repository].get_remote_user
      or not config.lf4rcs[repository].get_branches
  then
    log("Unsupported repository type")
    os.exit(1)
  end
  log("inspecting changesets")
  local remote_user = config.lf4rcs[repository].get_remote_user()
  local function abort(message)
    log("TEST FAILED: " .. message)
    log("ABORTING and ROLLBACK due to failed test.")
    db:query("ROLLBACK")
    os.exit(1)
  end
  db:query("BEGIN")
  local member = Member:new_selector()
    :add_where{ "login = ?", remote_user }
    :optional_object_mode()
    :exec()
  if not member then
    abort(
      "internal error, member '" 
      .. remote_user .. "' not found in database"
    )
  end
  local function exec(...)
    local command, output, err_message, exit_code = lf4rcs.exec(...)
    if not output then
      log("Could not execute: " .. command)
      abort(err_message)
    end
    if exit_code ~= 0 then
      log("Could not execute: " .. command)
      abort("Exit code: " .. tostring(exit_code))
    end
    return output
  end
  if config.lf4rcs[repository].extra_checks then
    local success, err_message = config.lf4rcs[repository].extra_checks(path, exec)
    if not success then
      abort(err_message)
    end
  end
  local branches, err = config.lf4rcs[repository].get_branches(path, exec)
  if not branches then abort(err) end
  for branch, head_node_ids in pairs(branches) do
    log('checking branch ' .. branch)
    if branch ~= config.lf4rcs[repository].working_branch_name then
      local initiative_id = string.match(branch, "^i([0-9]+)$")
      if not initiative_id 
          or initiative_id ~= tostring(tonumber(initiative_id))
      then
        abort("this branch name is not allowed")
      end
      initiative_id = tonumber(initiative_id)
      if #head_node_ids > 1 then
        abort("number of heads found for branch is greater than 1: " .. #head_node_ids)
      end
      local initiative = Initiative:by_id(initiative_id)
      if not initiative then
        abort("initiative i" .. initiative_id .. " not found" )
      end
      if initiative.issue.area.unit_id ~= tonumber(unit_id) then
        abort("initiative belongs to another unit (unit ID " .. initiative.issue.area.unit_id .. ")")
      end
      if initiative.issue.state ~= "admission" and initiative.issue.state ~= "discussion" then
        if initiative.issue.state == "verification" then
          if config.lf4rcs.push_grace_period then
            local in_push_grace_period = (Initiative:new_selector()
              :reset_fields()
              :add_field({ "now() - initiative.created <= ?", config.lf4rcs.push_grace_period }, "in_push_grace_period")
              :add_where{ "id = ?", initiative.id }
              :single_object_mode()
              :exec()).in_push_grace_period
            if not in_push_grace_period then
              abort("issue is already frozen and the push grace period is expired")
            end
          else
            abort("issue is already frozen")
          end
        else
          abort("issue is already closed (" .. initiative.issue.state .. ")")
        end
      end
      if initiative.revoked then
        abort("initiative has been revoked")
      end
      local initiator = Initiator:by_pk(initiative.id, member.id)
      if not initiator then
        abort("member is not initiator of initiative i" .. initiative_id)
      end
      if not initiator.accepted then
        abort(
          "member has not accepted invitation to become initiator of initiative i" 
          .. initiative_id
        )
      end
      local node_id = head_node_ids[1] or false
      if node_id then
        log("adding node " .. node_id .. " to initiative i" .. initiative_id)
      else
        log("removing node reference from initiative i" .. initiative_id)
      end
      Draft:update_content(member.id, initiative_id, nil, nil, node_id)
    end
  end
  log("changes cleared. continue committing.")
  db:query("COMMIT")
  os.exit(0)
end

