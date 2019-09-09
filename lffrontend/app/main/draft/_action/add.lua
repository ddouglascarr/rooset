return Draft:update_content(
  app.session.member.id, 
  param.get("initiative_id", atom.integer),
  param.get("formatting_engine"),
  param.get("content"),
  nil,
  param.get("preview") or param.get("edit")
)
