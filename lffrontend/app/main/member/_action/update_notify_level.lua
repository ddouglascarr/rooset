local disable_notifications = param.get("disable_notifications") == "true" and true or false

if app.session.member.disable_notifications ~= disable_notifications then
  IgnoredArea:destroy_by_member_id(app.session.member_id)
  app.session.member.disable_notifications = disable_notifications
end

if app.session.member.disable_notifications then
  app.session.member.notification_dow = nil
  app.session.member.notification_hour = nil
else 
  if param.get("digest") == "true" then
    local dow = param.get("notification_dow")
    if dow == "daily" then
      app.session.member.notification_dow = nil
    else
      app.session.member.notification_dow = tonumber(dow)
    end
    app.session.member.notification_hour = tonumber(param.get("notification_hour"))
  else
    app.session.member.notification_dow = nil
    app.session.member.notification_hour = nil
  end
end

app.session.member:save()
