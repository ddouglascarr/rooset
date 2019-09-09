Newsletter = mondelefant.new_class()
Newsletter.table = 'newsletter'

function Newsletter:send_next_newsletter()
  local newsletter_to_send = NewsletterToSend:get_next()
  if not newsletter_to_send then
    return false
  end

  local newsletter = newsletter_to_send.newsletter
  
  local newsletter_to_send = NewsletterToSend:by_newsletter_id(newsletter.id)
  newsletter_to_send:load("member")

  newsletter.sent = "now"
  newsletter:save()
  
  io.stderr:write("Sending newsletter " .. newsletter.id .. " to " .. (#newsletter_to_send) .. " members\n")
  
  for i, n in ipairs(newsletter_to_send) do
    
    local member = n.member
    
    if member.notify_email then
      io.stderr:write("Sending newsletter " .. newsletter.id .. " to " .. member.notify_email .. "\n")
      
      local success = net.send_mail{
        envelope_from = config.mail_envelope_from,
        from          = config.mail_from,
        reply_to      = config.mail_reply_to,
        to            = member.notify_email,
        subject       = newsletter.subject,
        content_type  = "text/plain; charset=UTF-8",
        content       = newsletter.content
      }
    end

  end
  
  return true
  
end