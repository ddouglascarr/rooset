
local initiative = param.get("initiative", "table")
local suggestions_selector = param.get("suggestions_selector", "table")

suggestions_selector:add_order_by("plus2_unfulfilled_count + plus1_unfulfilled_count DESC, id")

local tab_id = param.get("tab_id")
local show_name = param.get("show_name", atom.boolean)
if show_name == nil then
  show_name = true
end
local show_filter = param.get("show_filter", atom.boolean)
if show_filter == nil then
  show_filter = true
end

local partial = {
  routing = {
    default = {
      mode = "redirect",
      module = "initiative",
      view = "show_tab",
      params = {
        initiative_id = initiative.id,
        tab = "suggestions",
        tab_id = tab_id
      },
    }
  }
}

local ui_filters = ui.filters
if true or not show_filter then
  ui_filters = function(args) args.content() end
end

ui.container{ attr = { class = "box" },
  content = function()
    ui.paginate{
      selector = suggestions_selector,
      content = function()
        ui.list{
          attr = { style = "table-layout: fixed;" },
          records = suggestions_selector:exec(),
          columns = {
            {
              label = show_name and _"Suggestion" or nil,
              content = function(record)
                if show_name then
                  ui.link{
                    text = record.name,
                    module = "suggestion",
                    view = "show",
                    id = record.id
                  }
                end
              end
            },
            {
              label = _"Collective opinion of supporters",
              label_attr = { style = "width: 101px;" },
              content = function(record)
                if record.minus2_unfulfilled_count then
                  local max_value = record.initiative.supporter_count
                  ui.bargraph{
                    max_value = max_value,
                    width = 100,
                    bars = {
                      { color = "#0a0", value = record.plus2_unfulfilled_count + record.plus2_fulfilled_count },
                      { color = "#8f8", value = record.plus1_unfulfilled_count + record.plus1_fulfilled_count },
                      { color = "#eee", value = max_value - record.minus2_unfulfilled_count - record.minus1_unfulfilled_count - record.minus2_fulfilled_count - record.minus1_fulfilled_count - record.plus1_unfulfilled_count - record.plus2_unfulfilled_count - record.plus1_fulfilled_count - record.plus2_fulfilled_count},
                      { color = "#f88", value = record.minus1_unfulfilled_count + record.minus1_fulfilled_count },
                      { color = "#a00", value = record.minus2_unfulfilled_count + record.minus2_fulfilled_count },
                    }
                  }
                end
              end
            },
            {
              label = _"Suggestion currently not implemented",
              label_attr = { style = "width: 101px;" },
              content = function(record)
                if record.minus2_unfulfilled_count then
                  local max_value = record.initiative.supporter_count
                  ui.bargraph{
                    max_value = max_value,
                    width = 100,
                    bars = {
                      { color = "#0a0", value = record.plus2_unfulfilled_count },
                      { color = "#8f8", value = record.plus1_unfulfilled_count },
                      { color = "#eee", value = max_value - record.minus2_unfulfilled_count - record.minus1_unfulfilled_count - record.plus1_unfulfilled_count - record.plus2_unfulfilled_count },
                      { color = "#f88", value = record.minus1_unfulfilled_count },
                      { color = "#a00", value = record.minus2_unfulfilled_count },
                    }
                  }
                end
              end
            },
            {
              label = _"Suggestion currently implemented",
              label_attr = { style = "width: 101px;" },
              content = function(record)
                if record.minus2_fulfilled_count then
                  local max_value = record.initiative.supporter_count
                  ui.bargraph{
                    max_value = max_value,
                    width = 100,
                    bars = {
                      { color = "#0a0", value = record.plus2_fulfilled_count },
                      { color = "#8f8", value = record.plus1_fulfilled_count },
                      { color = "#eee", value = max_value - record.minus2_fulfilled_count - record.minus1_fulfilled_count - record.plus1_fulfilled_count - record.plus2_fulfilled_count},
                      { color = "#f88", value = record.minus1_fulfilled_count },
                      { color = "#a00", value = record.minus2_fulfilled_count },
                    }
                  }
                end
              end
            },
          }
        }
      end
    }
  end
}
