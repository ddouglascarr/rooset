Draft = mondelefant.new_class()
Draft.table = 'draft'

-- Many drafts belonging to an initiative
Draft:add_reference{
  mode          = 'm1',
  to            = "Initiative",
  this_key      = 'initiative_id',
  that_key      = 'id',
  ref           = 'initiative',
}

-- Many drafts are authored by a member
Draft:add_reference{
  mode          = 'm1',
  to            = "Member",
  this_key      = 'author_id',
  that_key      = 'id',
  ref           = 'author',
}

function Draft.object_get:author_name()
  return self.author and self.author.name or _"Unknown author"
end

model.has_rendered_content(Draft, RenderedDraft)

function Draft:update_content(member_id, initiative_id, p_formatting_engine, content, external_reference, preview)
  local initiative = Initiative:by_id(initiative_id)

  -- TODO important m1 selectors returning result _SET_!
  local issue = initiative:get_reference_selector("issue"):for_share():single_object_mode():exec()

  if issue.closed then
    slot.put_into("error", _"This issue is already closed.")
    return false
  elseif issue.half_frozen then 
    slot.put_into("error", _"This issue is already frozen.")
    return false
  elseif issue.phase_finished then
    slot.put_into("error", _"Current phase is already closed.")
    return false
  end

  local initiator = Initiator:by_pk(initiative.id, member_id)
  if not initiator or not initiator.accepted then
    error("access denied")
  end

  local tmp = db:query({ "SELECT text_entries_left FROM member_contingent_left WHERE member_id = ? AND polling = ?", member_id, initiative.polling }, "opt_object")
  if not tmp or tmp.text_entries_left < 1 then
    slot.put_into("error", _"Sorry, you have reached your personal flood limit. Please be slower...")
    return false
  end

  local formatting_engine
  if config.enforce_formatting_engine then
    formatting_engine = config.enforce_formatting_engine 
  else
    formatting_engine = p_formatting_engine

    local formatting_engine_valid = false
    for i, fe in pairs(config.formatting_engines) do
      if formatting_engine == fe.id then
        formatting_engine_valid = true
      end
    end
    if not formatting_engine_valid then
      error("invalid formatting engine!")
    end
  end

  if preview then
    return false
  end

  local old_draft = initiative.current_draft

  local draft = Draft:new()
  draft.author_id = member_id
  draft.initiative_id = initiative.id
  draft.formatting_engine = formatting_engine or old_draft and old_draft.formatting_engine or nil
  draft.content = content or old_draft and old_draft.content or nil
  if external_reference == false then
    draft.external_reference = nil
  else
    draft.external_reference = external_reference or old_draft and old_draft.external_reference or nil
  end
  draft:save()

  local supporter = Supporter:by_pk(initiative.id, member_id)

  if supporter then
    supporter.draft_id = draft.id
    supporter:save()
  end

  draft:render_content()

  slot.put_into("notice", _"The initiative text has been updated")
end
