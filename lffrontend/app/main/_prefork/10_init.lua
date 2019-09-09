config.app_version = "3.2.1"

-- TODO abstraction
-- get record by id
function mondelefant.class_prototype:by_id(id)
  local selector = self:new_selector()
  selector:add_where{ 'id = ?', id }
  selector:optional_object_mode()
  return selector:exec()
end

if not config.password_hash_algorithm then
  config.password_hash_algorithm = "crypt_sha512"
end

if not config.password_hash_min_rounds then
 config.password_hash_min_rounds = 10000
end

if not config.password_hash_max_rounds then
  config.password_hash_max_rounds = 20000
end

if config.use_terms_checkboxes == nil then
  config.use_terms_checkboxes = {}
end

if config.enabled_languages == nil then
  config.enabled_languages = { 'en', 'de', 'ka' } --, 'eo', 'el', 'hu', 'it', 'nl', 'zh-Hans', 'zh-TW' }
end

if config.default_lang == nil then
  config.default_lang = "en"
end

if config.mail_subject_prefix == nil then
  config.mail_subject_prefix = "[LiquidFeedback] "
end

if config.notification_digest_template == nil then
  config.notification_digest_template = "Hello #{name},\n\nthis is your personal digest.\n\n#{digest}\n"
end

if config.member_image_content_type == nil then
  config.member_image_content_type = "image/jpeg"
end

if config.member_image_convert_func == nil then
  config.member_image_convert_func = {
    avatar = function(data) return extos.pfilter(data, "convert", "jpeg:-", "-thumbnail",   "48x48", "jpeg:-") end,
    photo =  function(data) return extos.pfilter(data, "convert", "jpeg:-", "-thumbnail", "240x240", "jpeg:-") end
  }
end

if config.locked_profile_fields == nil then
  config.locked_profile_fields = {}
end

if config.check_delegations_default == nil then
  config.check_delegations_default = "confirm"
end

if config.ldap == nil then
  config.ldap = {}
end

if not config.database then
  config.database = { engine='postgresql', dbname='liquid_feedback' }
end

if config.fork == nil then
  config.fork = {}
end

if config.fork.pre == nil then
  config.fork.pre = 2
end

if config.fork.min == nil then
  config.fork.min = 4
end

if config.fork.max == nil then
  config.fork.max = 128
end

if config.fork.delay == nil then
  config.fork.delay = 0.125
end

if config.fork.error_delay == nil then
  config.fork.error_delay = 2
end

if config.fork.exit_delay == nil then
  config.fork.exit_delay = 2
end

if config.fork.idle_timeout == nil then
  config.fork.idle_timeout = 900
end

if config.port == nil then
  config.port = 8080
end

if config.localhost == nil then
  config.localhost = true
end

local listen_options = {
  pre_fork              = config.fork.pre,
  min_fork              = config.fork.min,
  max_fork              = config.fork.max,
  fork_delay            = config.fork.delay,
  fork_error_delay      = config.fork.error_delay,
  exit_delay            = config.fork.exit_delay,
  idle_timeout          = config.fork.idle_timeout,
  memory_limit          = config.fork.memory_limit,
  min_requests_per_fork = config.fork.min_requests,
  max_requests_per_fork = config.fork.max_requests,
  http_options          = config.http_options
}

if config.ipv6 then
  local host = config.localhost and "::1" or "::"
  listen_options[#listen_options+1] = { proto = "tcp", host = host, port = config.port }
end
if config.ipv6 ~= "only" then
  local host = config.localhost and "127.0.0.1" or "0.0.0.0"
  listen_options[#listen_options+1] = { proto = "tcp", host = host, port = config.port }
end

request.set_404_route{ module = 'index', view = '404' }

request.set_absolute_baseurl(config.absolute_base_url)

listen(listen_options)

listen{
  {
    proto = "interval",
    name  = "send_pending_notifications",
    delay = 5,
    handler = function()
      while true do
        if not Newsletter:send_next_newsletter() then
          break
        end
      end
      while true do
        if not Event:send_next_notification() then
          break
        end
      end
      while true do
        if not InitiativeForNotification:notify_next_member() then
          break
        end
      end
    end
  },
  min_fork = 1,
  max_fork = 1
}

execute.inner()

