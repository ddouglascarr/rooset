--[[--
locale.do_with(
  locale_options,  -- table with locale information (as if passed to locale.set(...))
  function()
    ...            -- code to be executed with the given locale settings
  end
)

This function executes code with temporarily changed locale settings. See locale.set(...) for correct usage of 'locale_options'.

--]]--

function locale.do_with(locale_options, block)
  local old_data = table.new(locale._current_data)
  if locale_options.reset then
    locale._current_data = {}
  end
  for key, value in pairs(locale_options) do
    if key ~= "reset" then
      locale._current_data[key] = value
    end
  end
  locale.set(locale_options)
  block()
  locale._current_data = old_data
end
