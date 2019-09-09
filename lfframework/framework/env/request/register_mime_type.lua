--[[--
request.register_mime_type(
  filename_extension,
  mime_type
)

Registers a mime type for a given filename extension (case sensitive).

--]]--

function request.register_mime_type(filename_extension, mime_type)
  request.configure(function()
    request._mime_types[filename_extension] = mime_type
  end)
end
