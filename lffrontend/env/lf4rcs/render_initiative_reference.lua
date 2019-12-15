function lf4rcs.render_initiative_reference(initiative, wrapper)
  if initiative.current_draft.external_reference then
    config.render_external_reference.draft(initiative.current_draft, wrapper)
  end
end

