Member = mondelefant.new_class()
Member.table = 'member'

local function secret_token()
  local parts = {}
  for i = 1, 5 do
    parts[#parts+1] = multirand.string(5, "23456789bcdfghjkmnpqrstvwxyz")
  end
  return (table.concat(parts, "-"))
end

Member:add_reference{
  mode          = "1m",
  to            = "MemberHistory",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'history_entries',
  back_ref      = 'member'
}

Member:add_reference{
  mode          = '1m',
  to            = "MemberImage",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'images',
  back_ref      = 'member'
}

Member:add_reference{
  mode          = '1m',
  to            = "Contact",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'contacts',
  back_ref      = 'member',
  default_order = '"other_member_id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Contact",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'foreign_contacts',
  back_ref      = 'other_member',
  default_order = '"member_id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Session",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'sessions',
  back_ref      = 'member',
  default_order = '"ident"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Draft",
  this_key      = 'id',
  that_key      = 'author_id',
  ref           = 'drafts',
  back_ref      = 'author',
  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Suggestion",
  this_key      = 'id',
  that_key      = 'author_id',
  ref           = 'suggestions',
  back_ref      = 'author',
  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Membership",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'memberships',
  back_ref      = 'member',
  default_order = '"area_id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Interest",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'interests',
  back_ref      = 'member',
  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Initiator",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'initiators',
  back_ref      = 'member'
}

Member:add_reference{
  mode          = '1m',
  to            = "Supporter",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'supporters',
  back_ref      = 'member'
}

Member:add_reference{
  mode          = '1m',
  to            = "Opinion",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'opinions',
  back_ref      = 'member',
  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Delegation",
  this_key      = 'id',
  that_key      = 'truster_id',
  ref           = 'outgoing_delegations',
  back_ref      = 'truster',
--  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Delegation",
  this_key      = 'id',
  that_key      = 'trustee_id',
  ref           = 'incoming_delegations',
  back_ref      = 'trustee',
--  default_order = '"id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "DirectVoter",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'direct_voter',
  back_ref      = 'member',
  default_order = '"issue_id"'
}

Member:add_reference{
  mode          = '1m',
  to            = "Vote",
  this_key      = 'id',
  that_key      = 'member_id',
  ref           = 'vote',
  back_ref      = 'member',
  default_order = '"issue_id", "initiative_id"'
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Member",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'contact',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'other_member_id',
  ref                   = 'saved_members',
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Member",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'contact',
  connected_by_this_key = 'other_member_id',
  connected_by_that_key = 'member_id',
  ref                   = 'saved_by_members',
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Unit",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'privilege',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'unit_id',
  ref                   = 'units'
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Area",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'membership',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'area_id',
  ref                   = 'areas'
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Issue",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'interest',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'issue_id',
  ref                   = 'issues'
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Initiative",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'initiator',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'initiative_id',
  ref                   = 'initiated_initiatives'
}

Member:add_reference{
  mode                  = 'mm',
  to                    = "Initiative",
  this_key              = 'id',
  that_key              = 'id',
  connected_by_table    = 'supporter',
  connected_by_this_key = 'member_id',
  connected_by_that_key = 'initiative_id',
  ref                   = 'supported_initiatives'
}

model.has_rendered_content(Member, RenderedMemberStatement, "statement")

function Member:build_selector(args)
  local selector = self:new_selector()
  if args.active ~= nil then
    selector:add_where{ "member.active = ?", args.active }
  end
  if args.locked ~= nil then
    selector:add_where{ "member.locked = ?", args.locked }
  end
  if args.is_contact_of_member_id then
    selector:join("contact", "__model_member__contact", "member.id = __model_member__contact.other_member_id")
    selector:add_where{ "__model_member__contact.member_id = ?", args.is_contact_of_member_id }
  end
  if args.voting_right_for_unit_id then
    selector:join("privilege", "__model_member__privilege", { "member.id = __model_member__privilege.member_id AND __model_member__privilege.voting_right AND __model_member__privilege.unit_id = ?", args.voting_right_for_unit_id })
  end
  if args.admin_search then
    local search_string = "%" .. args.admin_search .. "%"
    selector:add_where{ "member.identification ILIKE ? OR member.name ILIKE ?", search_string, search_string }
  end
  if args.order then
    if args.order == "id" then
      selector:add_order_by("id")
    elseif args.order == "identification" then
      selector:add_order_by("identification")
    elseif args.order == "name" then
      selector:add_order_by("name")
    else
      error("invalid order")
    end
  end
  return selector
end

function Member:lockForReference()
  self.get_db_conn().query("LOCK TABLE " .. self:get_qualified_table() .. " IN ROW SHARE MODE")
end


function Member:get_all_by_authority(authority)
  
  local members = Member:new_selector()
    :add_where{ "authority = ?", authority }
    :add_field("authority_uid")
    :exec()
    
  return members
end

function Member.object:set_password(password)
  trace.disable()
  
  local hash_prefix
  local salt_length

  local function rounds()
    return multirand.integer(
      config.password_hash_min_rounds,
      config.password_hash_max_rounds
    )
  end
      
  if config.password_hash_algorithm == "crypt_md5" then
    hash_prefix = "$1$" 
    salt_length = 8
    
  elseif config.password_hash_algorithm == "crypt_sha256" then
    hash_prefix = "$5$rounds=" .. rounds() .. "$"
    salt_length = 16
    
  elseif config.password_hash_algorithm == "crypt_sha512" then
    hash_prefix = "$6$rounds=" .. rounds() .. "$"
    salt_length = 16
    
  else
    error("Unknown hash algorithm selected in configuration")

  end
  
  hash_prefix = hash_prefix .. multirand.string(
    salt_length,
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./"
  )

  local hash = extos.crypt(password, hash_prefix)

  if not hash or hash:sub(1, #hash_prefix) ~= hash_prefix then
    error("Password hashing algorithm failed")
  end
  
  self.password = hash
  self.password_reset_secret = nil
  self.password_reset_secret_expiry = nil
end

function Member.object:check_password(password)
  if type(password) == "string" and type(self.password) == "string" then
    return extos.crypt(password, self.password) == self.password
  else
    return false
  end
end

function Member.object_get:password_hash_needs_update()
  
  if self.password == nil then
    return nil
  end

  local function check_rounds(rounds)
    if rounds then
      rounds = tonumber(rounds)
      if 
        rounds >= config.password_hash_min_rounds and 
        rounds <= config.password_hash_max_rounds
      then
        return false
      end
    end
    return true
  end
  
  if config.password_hash_algorithm == "crypt_md5" then

    return self.password:sub(1,3) ~= "$1$"
    
  elseif config.password_hash_algorithm == "crypt_sha256" then
    
    return check_rounds(self.password:match("^%$5%$rounds=([1-9][0-9]*)%$"))
    
  elseif config.password_hash_algorithm == "crypt_sha512" then

    return check_rounds(self.password:match("^%$6%$rounds=([1-9][0-9]*)%$"))

  else
    error("Unknown hash algorithm selected in configuration")

  end
  
end

function Member.object_get:published_contacts()
  return Member:new_selector()
    :join('"contact"', nil, '"contact"."other_member_id" = "member"."id"')
    :add_where{ '"contact"."member_id" = ?', self.id }
    :add_where("public")
    :exec()
end

function Member:by_login_and_password(login, password)
 
  local function prepare_login_selector()
    local selector = self:new_selector()
    selector:add_field({ "now() > COALESCE(last_delegation_check, activated) + ?::interval", config.check_delegations_interval_hard }, "needs_delegation_check_hard")
    selector:add_where('NOT "locked"')
    selector:optional_object_mode()
    return selector
  end
  
  local function do_local_login()
    local selector = prepare_login_selector()
    selector:add_where{'"login" = ?', login }
    local member = selector:exec()
    if member and member:check_password(password) then
      return member
    else
      return nil
    end
  end
  
  if config.ldap.member then

    -- Let's check the users credentials against the LDAP      
    local ldap_entry, ldap_err = ldap.check_credentials(login, password)

    -- Is the user already registered as member?
    local uid
    local selector = prepare_login_selector()

    -- Get login name from LDAP entry
    if ldap_entry then
      uid = config.ldap.member.uid_map(ldap_entry)
      selector:add_where{'"authority" = ? AND "authority_uid" = ?', "ldap", uid }

    -- or build it from the login
    else
      login = config.ldap.member.login_normalizer(login)
      selector:add_where{'"authority" = ? AND "authority_uid" = ?', "ldap", login }
    end
    
    local member = selector:exec()
    -- The member is already registered
    if member then

      -- The credentials entered by the user are invalid
      if ldap_err == "invalid_credentials" then
        
        -- Check if the user tried a cached password (which is invalid now)
        if config.ldap.member.cache_passwords and member:check_password(password) then
          member.password = nil
          member:save()
        end
        
        -- Try a regular login
        return do_local_login()

      end
      
      -- The credentials were accepted by the LDAP server and no error occured
      if ldap_entry and not ldap_err then
        
        -- Cache the password (if feature enabled)
        if config.ldap.member.cache_passwords and not member:check_password(password) then
          member:set_password(password)
        end

        -- update the member attributes and privileges from LDAP
        local ldap_conn, ldap_err, err, err2 = ldap.update_member_attr(member, nil, uid)
        if not err then
          local err = member:try_save()
          if err then
            return nil, "member_save_error", err
          end
          local succes, err, err2 = ldap.update_member_privileges(member, ldap_entry)
          if err then
            return nil, "update_member_privileges_error", err, err2
          end
          return member
        end

      end

      -- Some kind of LDAP error happened, if cached password are enabled,
      -- check user credentials against the cache
      if config.ldap.member.cache_passwords and member:check_password(password) then

        -- return the successfully logged in member
        return member

      end
      
    -- The member is not registered
    elseif config.ldap.member.registration and ldap_entry and not ldap_err then
      -- Automatic registration ("auto")
      if config.ldap.member.registration == "auto" then
        member = Member:new()
        member.authority = "ldap"
        local ldap_login
        if config.ldap.member.cache_passwords then 
          if config.ldap.member.login_normalizer then
            ldap_login = config.ldap.member.login_normalizer(login)
          else
            ldap_login = login
          end
        end
        -- TODO change this when SQL layers supports hstore
        member.authority_uid = uid
        member.authority_login = ldap_login
        member.activated = "now"
        member.last_activity = "now"
        if config.ldap.member.cache_passwords then
          member:set_password(password)
        end
        local ldap_conn, ldap_err, err, err2 = ldap.update_member_attr(member, nil, uid)
        if not err then
          local err = member:try_save()
          if err then
            return nil, "member_save_error", err
          end
          local success, err, err2 = ldap.update_member_privileges(member, ldap_entry)
          if err then
            return nil, "update_member_privileges_error", err, err2
          end
          return member
        end

      -- No automatic registration
      else
        return nil, "ldap_credentials_valid_but_no_member", uid
      end
    end
    
  end

  return do_local_login()
  
end

function Member:by_login(login)
  local selector = self:new_selector()
  selector:add_where{'"login" = ?', login }
  selector:optional_object_mode()
  return selector:exec()
end

function Member:by_name(name)
  local selector = self:new_selector()
  selector:add_where{'"name" = ?', name }
  selector:optional_object_mode()
  return selector:exec()
end

function Member:get_search_selector(search_string)
  return self:new_selector()
    :add_field( {'"highlight"("member"."name", ?)', search_string }, "name_highlighted")
    :add_where{ '"member"."text_search_data" @@ "text_search_query"(?)', search_string }
    :add_where("activated NOTNULL AND active")
end

function Member.object:send_password_reset_mail()
  trace.disable()
  if not self.notify_email then
    return false
  end
  self.password_reset_secret = secret_token()
  local expiry = db:query("SELECT now() + '1 days'::interval as expiry", "object").expiry
  self.password_reset_secret_expiry = expiry
  self:save()
  local content = slot.use_temporary(function()
    slot.put(_"Hello " .. self.name .. ",\n\n")
    slot.put(_"to reset your password please click on the following link:\n\n")
    slot.put(request.get_absolute_baseurl() .. "index/reset_password.html?secret=" .. self.password_reset_secret .. "\n\n")
    slot.put(_"If this link is not working, please open following url in your web browser:\n\n")
    slot.put(request.get_absolute_baseurl() .. "index/reset_password.html\n\n")
    slot.put(_"On that page please enter the reset code:\n\n")
    slot.put(self.password_reset_secret .. "\n\n")
  end)
  local success = net.send_mail{
    envelope_from = config.mail_envelope_from,
    from          = config.mail_from,
    reply_to      = config.mail_reply_to,
    to            = self.notify_email,
    subject       = config.mail_subject_prefix .. _"Password reset request",
    content_type  = "text/plain; charset=UTF-8",
    content       = content
  }
  return success
end

function Member.object:send_invitation(template_file, subject)
  trace.disable()
  self.invite_code = secret_token()
  self:save()
  
  local subject = subject
  local content
  
  if template_file then
    local fh = io.open(template_file, "r")
    content = fh:read("*a")
    content = (content:gsub("#{invite_code}", self.invite_code))
  else
    subject = config.mail_subject_prefix .. _"Invitation to LiquidFeedback"
    content = slot.use_temporary(function()
      slot.put(_"Hello\n\n")
      slot.put(_"You are invited to LiquidFeedback. To register please click the following link:\n\n")
      slot.put(request.get_absolute_baseurl() .. "index/register.html?invite=" .. self.invite_code .. "\n\n")
      slot.put(_"If this link is not working, please open following url in your web browser:\n\n")
      slot.put(request.get_absolute_baseurl() .. "index/register.html\n\n")
      slot.put(_"On that page please enter the invite key:\n\n")
      slot.put(self.invite_code .. "\n\n")
    end)
  end

  local success = net.send_mail{
    envelope_from = config.mail_envelope_from,
    from          = config.mail_from,
    reply_to      = config.mail_reply_to,
    to            = self.notify_email_unconfirmed or self.notify_email,
    subject       = subject,
    content_type  = "text/plain; charset=UTF-8",
    content       = content
  }
  return success
end

function Member.object:set_notify_email(notify_email)
  trace.disable()
  local expiry = db:query("SELECT now() + '7 days'::interval as expiry", "object").expiry
  self.notify_email_unconfirmed = notify_email
  self.notify_email_secret = secret_token()
  self.notify_email_secret_expiry = expiry
  local content = slot.use_temporary(function()
    slot.put(_"Hello " .. self.name .. ",\n\n")
    slot.put(_"Please confirm your email address by clicking the following link:\n\n")
    slot.put(request.get_absolute_baseurl() .. "index/confirm_notify_email.html?secret=" .. self.notify_email_secret .. "\n\n")
    slot.put(_"If this link is not working, please open following url in your web browser:\n\n")
    slot.put(request.get_absolute_baseurl() .. "index/confirm_notify_email.html\n\n")
    slot.put(_"On that page please enter the confirmation code:\n\n")
    slot.put(self.notify_email_secret .. "\n\n")
  end)
  local success = net.send_mail{
    envelope_from = config.mail_envelope_from,
    from          = config.mail_from,
    reply_to      = config.mail_reply_to,
    to            = self.notify_email_unconfirmed,
    subject       = config.mail_subject_prefix .. _"Email confirmation request",
    content_type  = "text/plain; charset=UTF-8",
    content       = content
  }
  if success then
    local lock_expiry = db:query("SELECT now() + '1 hour'::interval AS lock_expiry", "object").lock_expiry
    self.notify_email_lock_expiry = lock_expiry
  end
  self:save()
  return success
end

function Member.object:get_setting(key)
  return Setting:by_pk(self.id, key)
end

function Member.object:get_setting_value(key)
  local setting = Setting:by_pk(self.id, key)
  if setting then
    return setting.value
  end
end

function Member.object:set_setting(key, value)
  local setting = self:get_setting(key)
  if not setting then
    setting = Setting:new()
    setting.member_id = self.id
    setting.key = key
  end
  setting.value = value
  setting:save()
end

function Member.object:get_setting_maps_by_key(key)
  return SettingMap:new_selector()
    :add_where{ "member_id = ?", self.id }
    :add_where{ "key = ?", key }
    :add_order_by("subkey")
    :exec()
end

function Member.object:get_setting_map_by_key_and_subkey(key, subkey)
  return SettingMap:new_selector()
    :add_where{ "member_id = ?", self.id }
    :add_where{ "key = ?", key }
    :add_where{ "subkey = ?", subkey }
    :add_order_by("subkey")
    :optional_object_mode()
    :exec()
end

function Member.object:set_setting_map(key, subkey, value)
  setting_map = self:get_setting_map_by_key_and_subkey(key, subkey)
  if not setting_map then
    setting_map = SettingMap:new()
    setting_map.member_id = self.id
    setting_map.key = key
    setting_map.subkey = subkey
  end
  setting_map.value = value
  setting_map:save()
end

function Member.object_get:notify_email_locked()
  return(
    Member:new_selector()
      :add_where{ "id = ?", app.session.member.id }
      :add_where("notify_email_lock_expiry > now()")
      :count() == 1
  )
end

function Member.object_get:units_with_voting_right()
  return(Unit:new_selector()
    :join("privilege", nil, { "privilege.unit_id = unit.id AND privilege.member_id = ? AND privilege.voting_right", self.id })
    :exec()
  )
end

function Member.object:ui_field_text(args)
  args = args or {}
  if app.session:has_access("authors_pseudonymous") then
    -- ugly workaround for getting html into a replaced string and to the user
    ui.container{label = args.label, label_attr={class="ui_field_label"}, content = function()
        slot.put(string.format('<span><a href="%s">%s</a></span>',
                                                encode.url{
                                                  module    = "member",
                                                  view      = "show",
                                                  id        = self.id,
                                                },
                                                encode.html(self.name)))
      end
    }
  else
    ui.field.text{ label = args.label,      value = _"[not displayed public]" }
  end
end

function Member.object:has_voting_right_for_unit_id(unit_id)
  if not self.__units_with_voting_right_hash then
    local privileges = Privilege:new_selector()
      :add_where{ "member_id = ?", self.id }
      :add_where("voting_right")
      :exec()
    self.__units_with_voting_right_hash = {}
    for i, privilege in ipairs(privileges) do
      self.__units_with_voting_right_hash[privilege.unit_id] = true
    end
  end
  return self.__units_with_voting_right_hash[unit_id] and true or false
end

function Member.object:has_polling_right_for_unit_id(unit_id)
  if not self.__units_with_polling_right_hash then
    local privileges = Privilege:new_selector()
      :add_where{ "member_id = ?", self.id }
      :add_where("polling_right")
      :exec()
    self.__units_with_polling_right_hash = {}
    for i, privilege in ipairs(privileges) do
      self.__units_with_polling_right_hash[privilege.unit_id] = true
    end
  end
  return self.__units_with_polling_right_hash[unit_id] and true or false
end

function Member.object:get_delegatee_member(unit_id, area_id, issue_id)
  local selector = Member:new_selector()
  if unit_id then
    selector:join("delegation", nil, { "delegation.trustee_id = member.id AND delegation.scope = 'unit' AND delegation.unit_id = ? AND delegation.truster_id = ?", unit_id, self.id })
  end
  selector:optional_object_mode()
  return selector:exec()
end

function Member.object:delete()
  db:query{ "SELECT delete_member(?)", self.id }
end
