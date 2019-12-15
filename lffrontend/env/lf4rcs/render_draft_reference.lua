function lf4rcs.render_draft_reference(draft, wrapper)
  local repository, path, url = lf4rcs.get_config(draft.initiative.issue.area.unit)
  if not (config.lf4rcs[repository] and config.lf4rcs[repository].render_draft_reference) then
    error("Unsupported repository type")
  end
  if draft.external_reference then
    wrapper(function()
      config.lf4rcs[repository].render_draft_reference(url, draft)
    end)
  end
end

