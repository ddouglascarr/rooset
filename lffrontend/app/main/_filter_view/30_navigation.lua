slot.select ( 'navigation_right', function ()

  if app.session.member == nil then
    ui.link {
      text   = _"Login",
      module = 'index',
      view   = 'login',
      params = {
        redirect_module = request.get_module(),
        redirect_view = request.get_view(),
        redirect_id = param.get_id()
      }
    }
  end
  
  if app.session.member then
    ui.tag {
      tag = "div",
      content = function () 
        util.micro_avatar(app.session.member)
      end
    }
  end -- if app.session.member
    
end)

-- show notifications about things the user should take care of
if app.session.member then
  execute.view{
    module = "index", view = "_sidebar_notifications", params = {
      mode = "link"
    }
  }
end

slot.select ("footer", function ()
  if app.session.member_id and app.session.member.admin then
    ui.link {
      text   = _"System settings",
      module = 'admin',
      view   = 'index'
    }
    slot.put(" &middot; ")
  end
  ui.link{
    text   = _"About site",
    module = 'index',
    view   = 'about'
  }
  if config.use_terms then
    slot.put(" &middot; ")
    ui.link{
      text   = _"Use terms",
      module = 'index',
      view   = 'usage_terms'
    }
  end
  slot.put(" &middot; ")
  ui.link{
    text   = _"LiquidFeedback",
    external = "http://www.liquidfeedback.org/"
  }
end)

if not config.enable_debug_trace then
  trace.disable()
else
  slot.put_into('trace_button', '<div id="trace_show" onclick="document.getElementById(\'trace_content\').style.display=\'block\';this.style.display=\'none\';">TRACE</div>')
end


execute.inner()
