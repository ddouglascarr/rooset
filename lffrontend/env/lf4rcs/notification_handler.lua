function lf4rcs.notification_handler(event)
  if event.event == "issue_state_changed" and (
      event.state ~= "admission" and 
      event.state ~= "discussion" and 
      event.state ~= "verification" and
      event.state ~= "voting"
  ) then
    lf4rcs.commit(event.issue)
  end
end
  
