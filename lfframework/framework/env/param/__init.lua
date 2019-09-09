request.for_each(function()
  param._exchanged = false  -- important to be false, not nil
  param._saved = {}  -- stack
end)
