function jwt.encode(payload)
  local json_payload = encode.json(payload)
  local cmd = "/opt/etc/jwtencode '" .. json_payload .. "'"
  local enc = io.popen(cmd, "r")  
  local resp = ""
  for line in enc:lines() do
    resp = resp .. line
  end
  local a, b, code = enc:close()
  if code == 0 then return resp else error("failed to encode") end 
end

function jwt.decode(token)
  local dec = io.popen("/opt/etc/jwt-decode " .. token, "r")
  local resp = ""
  for line in dec:lines() do
    resp = resp .. line
  end
  local _, _, code = dec:close()
  local doc, _ = json.import(resp)
  if code == 0 then return doc else error("unauthorized") end
end
