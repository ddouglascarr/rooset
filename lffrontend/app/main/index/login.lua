slot.set_layout("rooset", "text/html")

app.html_title.title = _"Login"

ui.section(function() 
    ui.tag{
      tag = 'div',
      attr = { class = 'mx-auto max-w-xs w-full space-y-8' },
      content=function()
        ui.tag{
          tag = 'h2',
          attr = { class = 'mt-6 text-center text-xl font-extrabold text-gray-900' },
          content = 'Log In',
        }
        ui.form{
          module = 'index',
          action = 'login',
          attr = { class = 'px-4 pb-6 space-y-6 shadow-sm bg-gray-100 border rounded-md' },
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
            ui.newfield.text{
              attr = { id = "username_field" },
              label = _'Username',
              html_name = 'login',
              value  = ''
            }
            ui.newfield.text{
              attr = {
                type = 'password',
              },
              label     = _'Password',
              html_name = 'password',
              value     = ''
            }
            ui.newfield.button{
              attr = {
                type = "submit",
              },
              mode = "primary",
              content = _"Login",
            }
          end,
        }

      end,
    }
end )
