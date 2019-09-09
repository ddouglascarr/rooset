-- Escape a string to be used as safe LDAP filter
-- --------------------------------------------------------------------------
--
-- arguments:
--   filter: the string to be escaped (required)
--
-- returns:
--   escaped_filter: the escaped result

function ldap.escape_filter(filter)

  local null_pattern = (_VERSION == "Lua 5.1") and "%z" or "\000"
  
  return string.gsub(filter, "[\\%*%(%)\128-\255" .. null_pattern .. "]", function (char)
    
    return string.format("%02x", string.byte(char))
    
  end)
  
end