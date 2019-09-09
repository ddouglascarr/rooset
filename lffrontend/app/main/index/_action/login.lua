local login = param.get("login")
local password = param.get("password")

local member, err, uid = Member:by_login_and_password(login, password)

if err == "ldap_credentials_valid_but_no_member" then
  app.session.authority = "ldap"
  app.session.authority_uid = uid
  app.session.authority_login = login
  app.session:save()
  request.redirect{
    module = "index", view = "register", params = {
      ldap_login = login
    }
  }
  return
end

local function do_etherpad_auth(member)
  local result = net.curl(
    config.etherpad.api_base
    .. "api/1/createAuthorIfNotExistsFor?apikey=" .. config.etherpad.api_key
    .. "&name=" .. encode.url_part(member.name) .. "&authorMapper=" .. tostring(member.id)
  )
  
  if not result then
    slot.put_into("error", _"Etherpad authentication failed" .. " 1")
    return false
  end
  
  local etherpad_author_id = string.match(result, '"authorID"%s*:%s*"([^"]+)"')
  
  if not etherpad_author_id then
    slot.put_into("error", _"Etherpad authentication failed" .. " 2")
    return false
  end
  
  local time_in_24h = os.time() + 24 * 60 * 60
  
  local result = net.curl(
    config.etherpad.api_base 
    .. "api/1/createSession?apikey=" .. config.etherpad.api_key
    .. "&groupID=" .. config.etherpad.group_id
    .. "&authorID=" .. etherpad_author_id
    .. "&validUntil=" .. time_in_24h
  )

  if not result then
    slot.put_into("error", _"Etherpad authentication failed" .. " 3")
    return false
  end
  
  local etherpad_sesion_id = string.match(result, '"sessionID"%s*:%s*"([^"]+)"')

  if not etherpad_sesion_id then
    slot.put_into("error", _"Etherpad authentication failed" .. " 4")
    return false
  end

  request.set_cookie{
    path = config.etherpad.cookie_path,
    name = "sessionID",
    value = etherpad_sesion_id
  }
end

if member then
  member.last_login = "now"
  
  local delegations = Delegation:delegations_to_check_for_member_id(member.id)
  
  if config.check_delegations_interval_hard 
      and member.needs_delegation_check_hard
      and #delegations > 0 then
        
    app.session.needs_delegation_check = true
    
  else
    
    if #delegations == 0 then
      member.last_delegation_check = "now"
    end
    
    member.last_activity = "now"
    member.active = true
    
  end
  
  if member.lang == nil then
    member.lang = app.session.lang
  else
    app.session.lang = member.lang
  end

  if member.password_hash_needs_update then
    member:set_password(password)
  end
  
  member:save()
  app.session.member = member
  app.session:save()
  trace.debug('User authenticated')
  if config.etherpad then
    do_etherpad_auth(member)
  end
  slot.select("script", function()
    ui.script{ script = [[
      $('#swiper_info').addClass('active');
    ]] }
  end)
  slot.select("swiper_info", function()
    ui.tag { content = _"select tabs" }
    slot.put(" &uparrow; ")
    ui.tag { content = _"or swipe" }
    slot.put(" &leftarrow;<br />")
    ui.tag { content = _"to show more info and learn what you can do" }
  end )
else
  slot.select("error", function()
    ui.tag{ content = _'Invalid login name or password!' }
  end)
  trace.debug('User NOT authenticated')
  return false
end
