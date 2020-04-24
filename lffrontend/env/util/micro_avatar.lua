function util.micro_avatar(member, member_name)
  if type ( member ) == "number" then
    member = {
      id = member,
      name = member_name
    }
  end
  
  local function doit()
    ui.tag { tag = "i", attr = { class = "material-icons" }, content = "person" }

    -- lf provided avitar functionality
    -- if config.fastpath_url_func then
    --   ui.image{
    --     attr = {
    --       title = member.name,
    --       class = "microAvatar"
    --     },
    --     external = config.fastpath_url_func(member.id, "avatar")
    --   }
    -- else
      -- ui.image {
      --   attr = {
      --     title = member.name,
      --     class = "microAvatar"
      --   },
      --   module = "member_image",
      --   view = "show",
      --   extension = "jpg",
      --   id = member.id,
      --   params = {
      --     image_type = "avatar"
      --   }
      -- } 
    -- end
  end
  
  ui.link {
    content = function ()
      if app.session:has_access("everything") then
        ui.link {
          module = "member", view = "show", id = member.id,
          content = doit
        }
      else
        ui.tag{ content = doit }
      end
    end
  }
end
