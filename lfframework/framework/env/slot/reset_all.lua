--[[--
slot.reset_all{
  except =  except  -- Reset all slots, except slots named in this list
}

Calling this function resets all slots to be empty. An exclusion list may be passed to the function as named argument.

--]]--

local data_metatable = {}
function data_metatable:__index(key)
  self[key] = { string_fragments = {}, state_table = {} }
  return self[key]
end

function slot.reset_all(args)
  local saved
  if args and args.except then
    saved = {}
    for i, key in ipairs(args.except) do
      saved[key] = slot._data[key]
    end
  end
  slot._data = setmetatable({}, data_metatable)
  if saved then
    for key, value in pairs(saved) do
      slot._data[key] = value
    end
  end
end
