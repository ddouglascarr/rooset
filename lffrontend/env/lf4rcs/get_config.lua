function lf4rcs.get_config(unit)
  if not unit.external_reference then
    error("Unit is not configured for lf4rcs")
  end
  local repository, path, url = string.match(unit.external_reference, "([^ ]+) ([^ ]+) (.*)")
  return repository, path, url
end

