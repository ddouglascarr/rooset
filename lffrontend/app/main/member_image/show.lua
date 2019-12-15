local image_type = param.get("image_type")
local record = MemberImage:by_pk(param.get_id(), image_type, true)

local dynamic = param.get("dynamic")

local data, content_type

if record then
  data = record.data
  assert(record.content_type, "No content-type set for image.")
  content_type = record.content_type

else
  local default_file = "avatar.jpg"
  content_type = "image/jpeg"
  if image_type == "photo" then
    default_file = "icons/16/lightning.png"
    content_type = "image/png"
  end
  
  local filename = WEBMCP_BASE_PATH .. "static/" .. default_file
  
  local f = assert(io.open(filename), "Cannot open default image file")
  data = f:read("*a")
  f:close()

end

if not dynamic then
  request.allow_caching()
end

slot.set_layout(nil, content_type)
slot.put_into("data", data)
