#!/usr/bin/env lua
for line in io.lines() do
  local ident, code = line:match("^#define[ \t]+LDAP_([A-Z][A-Z_]*)[ \t]+([^ \t/]+)")
  if ident then
    io.stdout:write('  {"', ident:lower(), '", ', tonumber(code) or code, '},\n')
  end
end
