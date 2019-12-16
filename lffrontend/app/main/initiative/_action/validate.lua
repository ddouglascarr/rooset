local doc = param.get("doc", atom.string)

-- TODO: send this to the validator
slot.put_into("notice", doc)

request.redirect{
  module = "initiative",
  view = "new",
  params = { area_id = param.get("area_id", atom.integer) },
} 


