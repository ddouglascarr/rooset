if not config.download_dir then
  error("feature not enabled")
end

local file_list = extos.listdir(config.download_dir)

local tmp = {}
for i, filename in ipairs(file_list) do
  if not filename:find("^%.") then
    tmp[#tmp+1] = filename
  end
end

local file_list = tmp

table.sort(file_list, function(a, b) return a > b end)


ui.title(_"Download database export")

ui.section( function()

  ui.sectionHead( function()
    ui.heading { level = 1, content = _"Download database export" }
  end )
  
  if config.download_use_terms then
    ui.sectionRow( function()
      ui.container{
        attr = { class = "wiki use_terms" },
        content = function()
          slot.put(config.download_use_terms)
        end
      }
    end )
  end
  
  ui.sectionRow( function()

    ui.list{
      records = file_list,
      columns = {
        {
          content = function(filename)
            ui.tag{ content = filename }
          end
        },
        {
          content = function(filename)
            ui.link{
              content = _"Download",
              module = "index",
              view = "download_file",
              params = { filename = filename }
            }
          end
        }
      }
    }
  end)
end)