function trace._new_entry(node)
  local node = node or {}
  local stack = trace._stack
  local parent = stack[#stack]
  parent[#parent+1] = node
  return node
end
