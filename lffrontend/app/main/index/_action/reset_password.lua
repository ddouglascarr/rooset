trace.disable()
    
local secret = param.get("secret")

if not secret then

  local member = Member:new_selector()
    :add_where{ "login = ?", param.get("login") }
    :add_where("password_reset_secret ISNULL OR password_reset_secret_expiry < now()")
    :optional_object_mode()
    :exec()

  if member then
    if not member.notify_email then
      slot.put_into("error", _"Sorry, but there is not confirmed email address for your account. Please contact the administrator or support.")
      return false
    end
    member:send_password_reset_mail()
  end

  slot.put_into("notice", _"Your request has been processed.")

else
  local member = Member:new_selector()
    :add_where{ "password_reset_secret = ?", secret }
    :add_where{ "password_reset_secret_expiry > now()" }
    :optional_object_mode()
    :exec()

  if not member then
    slot.put_into("error", _"Reset code is invalid!")
    return false
  end

  local password1 = param.get("password1")
  local password2 = param.get("password2")

  if password1 ~= password2 then
    slot.put_into("error", _"Passwords don't match!")
    return false
  end

  if #password1 < 8 then
    slot.put_into("error", _"Passwords must consist of at least 8 characters!")
    return false
  end

  member:set_password(password1)
  member:save()

  slot.put_into("notice", _"Password has been reset successfully")

end
