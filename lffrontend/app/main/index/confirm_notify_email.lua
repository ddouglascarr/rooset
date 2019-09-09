ui.title(_"Email address confirmation")

ui.section(function() 

  ui.sectionHead(function()

    ui.form{
      attr = { class = "vertical" },
      module = "index",
      action = "confirm_notify_email",
      routing = {
        ok = {
          mode = "redirect",
          module = "index",
          view = "index"
        }
      },
      content = function()
        ui.field.text{
          label = _"Confirmation code",
          name = "secret",
          value = param.get("secret")
        }
        ui.submit{ text = _"Confirm" }
      end
    }

  end)
end)