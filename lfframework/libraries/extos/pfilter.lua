--[[

This file contains an alternative pfilter(...) implementation in pure Lua (utilizing moonbridge_io). This implementation is currently not used since extos already comes with a C implementation (extos.pfilter), which is independent of moonbridge_io and thus preferred to the implementation below.

data_out,    -- string containing stdout data, or nil in case of error
data_err,    -- string containing error or stderr data
status =     -- exit code, or negative code in case of abnormal termination
pfilter(
  data_in,   -- string containing stdin data
  filename,  -- executable
  arg1,      -- first (non-zero) argument to executable
  arg2,      -- second argument to executable
  ...
)

Executes the executable given by "filename", passing optional arguments. A given string may be fed into the program as stdin. On success 3 values are returned: A string containing all stdout data of the sub-process, a string containing all stderr data of the sub-process, and a status code. The status code is negative, if the program didn't terminate normally. By convention a status code of zero indicates success, while positive status codes indicate error conditions. If program execution was not possible at all, then nil is returned as first value and an error string as second value.

--]]

return function(data_in, ...)
  local process, errmsg = moonbridge_io.exec(...)
  if not process then return nil, errmsg end
  local read_fds = {[process.stdout] = true, [process.stderr] = true}
  local write_fds = {[process.stdin] = true}
  local function read(socket, chunks)
    if read_fds[socket] then
      local chunk, status = socket:read_nb()
      if not chunk then
        socket:close()
        read_fds[socket] = nil
      else
        chunks[#chunks+1] = chunk
        if status == "eof" then
          socket:close()
          read_fds[socket] = nil
        end
      end
    end
  end
  local function write(...)
    if write_fds[process.stdin] then
      local buffered = process.stdin:flush_nb(...)
      if not buffered or buffered == 0 then
        process.stdin:close()
        write_fds[process.stdin] = nil
      end
    end
  end
  write(data_in or "")
  local stdout_chunks, stderr_chunks = {}, {}
  while next(read_fds) or next(write_fds) do
    moonbridge_io.poll(read_fds, write_fds)
    read(process.stdout, stdout_chunks)
    read(process.stderr, stderr_chunks)
    write()
  end
  return
    table.concat(stdout_chunks),
    table.concat(stderr_chunks),
    process:wait()
end

