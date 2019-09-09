function trace._open_section(node)
  local node = trace._new_entry(node)
  node.section = true
  node.db_time = 0
  node.start_real_time = extos.monotonic_hires_time()
  node.start_proc_time = os.clock()
  local stack = trace._stack
  stack[#stack+1] = node
  return node
end
