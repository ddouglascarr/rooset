slot.set_layout("rooset", "text/html")
local unit = app.unit

local areas = Area:new_selector()
  :reset_fields()
  :add_field("area.id", nil, { "grouped" })
  :add_field("area.unit_id", nil, { "grouped" })
  :add_field("area.name", nil, { "grouped" })
  :add_where{ "area.unit_id = ?", unit.id }
  :add_where{ "area.active" }
  :add_order_by("area.name")
  :exec()

slot.put_into("default", ui.tag{
  tag = "div",
  attr = {
    class = 'bg-white border-2 border-grey-700 rounded px-2',
  },
  content = function()
    ui.tag{
      tag = 'h3',
      attr = {
        class = 'text-lg font-semibold',
      },
        content = 'Documents',
    }
    ui.tag{
      tag = 'ul',
      attr = { class = '' },
      content = function()
        for i, area in ipairs(areas) do
          ui.tag{
            tag = 'li',
            attr = { class = 'pl-2' },
            content = function()
              ui.link{
                attr = { class = 'text-blue-600 underline' },
                module = 'area',
                view = 'document',
                params = { area_id = area.id },
                content = area.name,
              }
            end,
          }
        end
      end,
    }
  end,
})

-- local function getIssuesSelector()
--   return Issue:new_selector()
--     :add_order_by([[
--       coalesce(
--         issue.fully_frozen + issue.voting_time, 
--         issue.half_frozen + issue.verification_time, 
--         issue.accepted + issue.discussion_time, 
--         issue.created + issue.max_admission_time
--       ) - now()
--     ]])
-- end
-- 
-- if app.session:has_access("anonymous") then
--   -- show the units the member has voting privileges for
--   execute.view {
--     module = "index", view = "_sidebar_units", params = {
--       member = app.session.member
--     }
--   }
-- end
-- 
-- -- show active members
-- if app.session:has_access("all_pseudonymous") then
--   execute.view{ module = "index", view = "_sidebar_members" }
-- end
-- 
-- if app.session:has_access("anonymous") then
--   
--   -- execute.view {
--   --   module = "issue", view = "_list2", params = { }
--   -- }
--   
-- end -- if app.session:has_access "anonymous"
