function lf4rcs.exec(...)
  local output, err_message, exit_code = extos.pfilter(nil, ...)
  local command_parts = {...}
  for i, part in ipairs(command_parts) do
    if string.match(part, " ") then
      command_parts[i] = '"' .. part .. '"'
    end
  end
  local command = table.concat(command_parts, " ")
  return command, output, err_message, exit_code
end

