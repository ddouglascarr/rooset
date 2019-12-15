function execute._create_sorted_execution_list(add_paths, execute_func)
  local entries = {}
  add_paths(function(...)
    local full_path     = encode.file_path(WEBMCP_BASE_PATH, "app", ...)
    local relative_path = encode.file_path("", ...)
    local filenames = extos.listdir(full_path)
    if filenames then
      table.sort(filenames)  -- not really neccessary, due to sorting afterwards
      for i, filename in ipairs(filenames) do
        if string.find(filename, "%.lua$") then
          if entries[filename] then
            error('More than one filter or initializer is named "' .. filter_name .. '".')
          end
          entries[#entries+1] = filename
          entries[filename] = function()
            execute_func(
              encode.file_path(full_path, filename),
              encode.file_path(relative_path, filename)
            )
          end
        end
      end
    end
  end)
  table.sort(entries)
  for idx, filename in ipairs(entries) do
    entries[idx] = entries[filename]
    entries[filename] = nil
  end
  return entries
end
