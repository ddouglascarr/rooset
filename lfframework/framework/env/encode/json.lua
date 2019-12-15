--[[--
json_string =  -- JavaScript code
encode.json(
  value        -- nil, false, true, a number, a string, or json.array{...} or json.object{...}
)

This function encodes any native datatype or table structure to JavaScript object notation (JSON). In order to distinguish between the empty array and the empty object, use json.array{} and json.object{} respectively.

The return value of this function is additionally escaped in such way that it can be safely included in inline scripts both in HTML and XHTML (within CDATA section).

--]]--

function encode.json(obj)
  local str = json.export(obj)
  str = string.gsub(str, "</", "<\\/")
  str = string.gsub(str, "<!%[CDATA%[", "\\u003c![CDATA[")
  str = string.gsub(str, "]]>", "]]\\u003e")
  return str
end
