function trace._close_section()
  local pos = #trace._stack
  local closed_section = trace._stack[pos]
  if not closed_section then
    error("All trace sections have been closed already.")
  end
  closed_section.stop_real_time = extos.monotonic_hires_time()
  closed_section.stop_proc_time = os.clock()
  trace._stack[pos] = nil
  return closed_section
end
