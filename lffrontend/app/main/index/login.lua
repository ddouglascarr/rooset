ui.tag{
  tag = "noscript",
  content = function()
    slot.put(_"JavaScript is disabled or not available.")
  end
}

app.html_title.title = _"Login"

execute.view{ module = "index", view = "_sidebar_motd_public" }

ui.section(function() 

ui.form{
  module = 'index',
  action = 'login',
  attr = { class = 'pure-form pure-form-stacked' },
  routing = {
    ok = {
      mode   = 'redirect',
      module = param.get("redirect_module") or "index",
      view = param.get("redirect_view") or "index",
      id = param.get("redirect_id"),
    },
    error = {
      mode   = 'forward',
      module = 'index',
      view   = 'login',
    }
  },
  content = function()
    ui.tag {
      tag = 'fieldset',
      content = function()
        ui.sectionRow(function()
          ui.field.text{
            attr = { id = "username_field" },
            label     = _'Login name',
            name = 'login',
            value     = ''
          }
          ui.field.password{
            label     = _'Password',
            name = 'password',
            value     = ''
          }
          ui.container { attr = { class = "actions" }, content = function()
            ui.tag{
              tag = "input",
              attr = {
                type = "submit",
                class = "pure-button pure-button-primary",
                value = _'Login'
              },
              content = ""
            }
            -- slot.put("<br />")
            -- slot.put("<br />")
            -- ui.link{ module = "index", view = "reset_password", text = _"Forgot password?" }
            -- slot.put("&nbsp;&nbsp;")
            -- ui.link{ module = "index", view = "send_login", text = _"Forgot login name?" }
          end }
        end )
      end
    }
  end
}
end )
