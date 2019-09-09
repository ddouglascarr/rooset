function lf4rcs.commit(issue)
  local repository, path, url = lf4rcs.get_config(issue.area.unit)
  if not (config.lf4rcs[repository] and config.lf4rcs[repository].commit) then
    error("Unsupported repository type")
  end
  local initiatives = Initiative:new_selector()
    :add_where{ "issue_id = ?", issue.id }
    :exec()
  for i, initiative in ipairs(initiatives) do
    local failure = false
    local function exec(...)
      local command, output, err_message, exit_code = lf4rcs.exec(...)
      local log
      if failure then
        log = "Skipped: " .. command .. "\n"
      else
        log = "Executed: " .. command .. "\n"
        if output then
          log = log .. output .. "\n"
        end
        if err_message and #err_message > 0 then
          log = log .. "ERROR: " .. err_message .. "\n"
          failure = true
        end
        if exit_code and exit_code ~= 0 then
          log = log .. "Exit code: " .. tostring(exit_code) .. "\n"
          failure = true
        end
      end
      issue.admin_notice = (issue.admin_notice or "") .. log
      issue:save()
    end
    local close_message, merge_message
    if initiative.winner then
      close_message = "Initiative i" .. initiative.id
                      .. " accepted as winner. Closing branch."
      merge_message = "Initiative i" .. initiative.id 
                      .. " accepted as winner. Applying branch changesets to upstream."
    else
      close_message = "Initiative i" .. initiative.id .. " rejected. Closing branch."
    end
    local target_node_id = initiative.current_draft.external_reference
    if target_node_id then
      local branch = "i" .. initiative.id
      lf4rcs[repository].commit(path, exec, branch, target_node_id, close_message, merge_message)
    end
  end
end
