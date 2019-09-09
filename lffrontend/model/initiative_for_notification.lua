InitiativeForNotification = mondelefant.new_class()
InitiativeForNotification.table = 'initiative_for_notification'

InitiativeForNotification:add_reference{
  mode          = 'm1',
  to            = "Initiative",
  this_key      = 'initiative_id',
  that_key      = 'id',
  ref           = 'initiative',
}


function InitiativeForNotification:notify_member_id(member_id)
  
  db:query("BEGIN TRANSACTION ISOLATION LEVEL REPEATABLE READ")
  
  local member = Member:by_id(member_id)

  locale.set{ lang = member.lang or config.default_lang }
 
  local selector = db:new_selector()
    :add_field("*")
    :from({ "get_initiatives_for_notification(?)", member_id }, "initiative_for_notification")
    :join("initiative", nil, "initiative.id = initiative_for_notification.initiative_id")
    :join("issue", nil, "issue.id = initiative.issue_id")
    :join("member", nil, "member.id = initiative_for_notification.recipient_id")
    :add_order_by("md5(initiative_for_notification.recipient_id || '-' || member.notification_counter || '-' || issue.area_id)")
    :add_order_by("md5(initiative_for_notification.recipient_id || '-' || member.notification_counter || '-' || issue.id)")

  selector._class = self

  local initiatives_for_notification = selector:exec()

  db:query("COMMIT")
  
  if not member.notify_email then
    return
  end
  
  if #initiatives_for_notification < 1 then
    return
  end
  
  io.stderr:write("Sending digest #" .. member.notification_counter .. " to " .. member.notify_email .. "\n")
  
  local initiatives = initiatives_for_notification:load("initiative")
  local issues = initiatives:load("issue")
  issues:load("area")
  issues:load("policy")

  local last_area_id
  local last_issue_id
  
  local draft_count = 0
  local suggestion_count = 0
  
  local draft_initiative
  local suggestion_initiative
  
  local ms = {}
  
  for i, entry in ipairs(initiatives_for_notification) do
    local initiative = entry.initiative
    local issue = initiative.issue
    local area = issue.area
    local policy = issue.policy
    
    local m = {}
    if last_area_id ~= area.id then
      m[#m+1] = "*** " .. area.name .. " ***"
      m[#m+1] = ""
      last_area_id = area.id
    end
    if last_issue_id ~= issue.id then
      local state_time_text
      if string.sub(issue.state_time_left, 1, 1) == "-" then
        state_time_text = _"Phase ends soon"
      else
        state_time_text = _( "#{interval} left", {
          interval = format.interval_text(issue.state_time_left)
        })
      end
      m[#m+1] = "---"
      m[#m+1] = policy.name .. " #" .. issue.id .. " - " .. issue.state_name .. " - " .. state_time_text
      m[#m+1] = ""
      last_issue_id = issue.id
    end
    m[#m+1] = initiative.display_name
    local source
    if entry.supported then
      source = _"has my support"
      if entry.new_draft then
        source = source .. ", " .. _"draft updated"
        draft_count = draft_count + 1
        draft_initiative = initiative
      end
      if entry.new_suggestion_count then
        if entry.new_suggestion_count == 1 then
          source = source .. ", " .. _"new suggestion added"
        elseif entry.new_suggestion_count > 1 then
          source = source .. ", " .. _("#{count} suggestions added", { count = entry.new_suggestion_count })
        end
        suggestion_count = suggestion_count + entry.new_suggestion_count
        if suggestion_initiative and suggestion_initiative.id ~= initiative.id then
          suggestion_initiative = false
        elseif suggestion_initiative ~= false then
          suggestion_initiative = initiative
        end
      end
    elseif entry.featured then
      source = _"featured"
    end
    if entry.leading then
      source = source and source .. ", " or ""
      source = source .. "currently leading"
    end
    m[#m+1] = "(" .. source .. ")"
    m[#m+1] = ""
    if not initiatives_for_notification[i+1] or initiatives_for_notification[i+1].issue_id ~= issue.id then
      m[#m+1] = _("more: #{url}", { url = request.get_absolute_baseurl() .. "issue/show/" .. issue.id .. ".html" })
      m[#m+1] = ""
    end
    ms[#ms+1] = table.concat(m, "\n")
  end

  local info = {}
    
  if draft_count > 0 then
    if draft_count == 1 then
      info[#info+1] = _"draft updated for " .. draft_initiative.display_name
    else
      info[#info+1] = _("drafts of #{draft_count} initiatives updated", { draft_count = draft_count })
    end
  end
  
  if suggestion_count > 0 then
    if suggestion_count == 1 then
      info[#info+1] = _"new suggestion for " .. suggestion_initiative.display_name
    elseif suggestion_initiative then
      info[#info+1] = _("#{count} suggestions added for #{initiative}", { count = suggestion_count, suggestion_initiative.display_name })
    else
      info[#info+1] = _("#{count} suggestions added", { count = suggestion_count })
    end
  end
  
  if draft_count == 0 and suggestion_count == 0 then
    info[#info+1] = _"featured initiatives"
  end
  
  local subject = _("Digest #{id}: #{info}", {
    id = member.notification_counter, info = table.concat(info, ", ")
  })
  
  local template = config.notification_digest_template
  
  local message = _(template, {
    name = member.name,
    digest = table.concat(ms, "\n")
  })
  
  local success = net.send_mail{
    envelope_from = config.mail_envelope_from,
    from          = config.mail_from,
    reply_to      = config.mail_reply_to,
    to            = member.notify_email,
    subject       = subject,
    content_type  = "text/plain; charset=UTF-8",
    content       = message
  }

end

function InitiativeForNotification:notify_next_member()
  local scheduled_notification_to_send = ScheduledNotificationToSend:get_next()
  if not scheduled_notification_to_send then
    return false
  end
  InitiativeForNotification:notify_member_id(scheduled_notification_to_send.recipient_id)
  return true
end

