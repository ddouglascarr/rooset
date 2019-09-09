if not app.session.member.admin then
  error('access denied')
end

if config.admin_logger then
  config.admin_logger(request.get_param_strings())
end

execute.inner()
