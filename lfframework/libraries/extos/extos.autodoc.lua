
--[[--
data_out,               -- string containing stdout data, or nil in case of error
data_err,               -- string containing error or stderr data
status =                -- exit code, or negative code in case of abnormal termination
extos.pfilter(
  data_in,   -- string containing stdin data
  filename,  -- executable
  arg1,      -- first (non-zero) argument to executable
  arg2,      -- second argument to executable
  ...
)

Executes the executable given by "filename", passing optional arguments. A given string may be fed into the program as stdin. On success 3 values are returned: A string containing all stdout data of the sub-process, a string containing all stderr data of the sub-process, and a status code. The status code is negative, if the program didn't terminate normally. By convention a status code of zero indicates success, while positive status codes indicate error conditions. If program execution was not possible at all, then nil is returned as first value and an error string as second value.

--]]--
-- implemented in extos.c as
-- static int extos_pfilter(lua_State *L)
--//--


--[[--
directory_entries,  -- table of directory entries
errmsg =            -- error message if directory could not be read
extos.listdir(
  path              -- path name
)

This function returns a table containing strings representing each entry in a directory. On error nil and an error message are returned.

--]]--
-- implemented in extos.c as
-- static int extos_listdir(lua_State *L)
--//--


--[[--
filestat_table,  -- table with information on the file, false if file does not exist, nil on error
errmsg =         -- error message if file information could not be read or file does not exist
extos.stat(
  filename       -- path to a file on the file system
)

Return information on a file, following symbolic links if applicable. See also: extos.lstat(...) and extos.fstat(...).

The returned table contains the following fields:

- "dev" (numeric ID of the device containing the file)
- "ino" (file's inode number)
- "nlink" (number of hard links to the file)
- "atime" (time when file data was last accessed)
- "mtime" (time when file data was last modified)
- "ctime" (time when file status was last changed)
- "size" (file size in bytes)
- "blksize" (optimal I/O block size for the file)
- "blocks" (actual number of blocks allocated for the file in 512-byte units)
- "uid" (user ID of the file's owner)
- "gid" (group ID of the file)
- "mode" (bitfield including the access permissions)
- "isblk" (true if block special file)
- "ischr" (true if character special file)
- "isdir" (true if directory)
- "isfifo" (true if pope of FIFO special file)
- "islnk" (true if symbolic link)
- "isreg" (true if regular file)
- "issock" (true if socket)

If the file does not exist, false and an error message are returned.
In case of any other error, nil and an error message are returned.


--]]--
-- implemented in extos.c as
-- static int extos_stat(lua_State *L)
--//--


--[[--
filestat_table,  -- table with information on the file, false if file does not exist, nil on error
errmsg =         -- error message if file information could not be read or file does not exist
extos.lstat(
  filename       -- path to a file on the file system
)

Return information on a file. Symbolic links are not followed, which means that if the filename points to a symbolic link, information on that symbolic link will be returned. Otherwise this function behaves like extos.stat(filename).

See extos.stat(...) for further information.

--]]--
-- implemented in extos.c as
-- static int extos_stat(lua_State *L)
--//--


--[[--
filestat_table,  -- table with information on the file, nil on error
errmsg =         -- error message if file information could not be determined
extos.fstat(
  file_handle    -- Lua file handle (e.g. as returned by io.open(...))
)

Return information on an open file. The file is specified by passing an open file handle to this function. Otherwise this function behaves like extos.stat(...).

See extos.stat(...) for further information.

--]]--
-- implemented in extos.c as
-- static int extos_stat(lua_State *L)
--//--


--[[--
passhash =     -- encrypted password
extos.crypt{
  key = key,   -- password to be one-way encrypted
  salt = salt  -- salt to be used for encryption, optionally starting with "$N$", where N is a digit
}

This function is a wrapper for the C function char *crypt(const char *key, const char *salt).

--]]--
-- implemented in extos.c as
-- static int extos_crypt(lua_State *L)
--//--


--[[--
seconds =
extos.hires_time()

Returns a unix time stamp representing current time with high resolution.

--]]--
-- implemented in extos.c as
-- static int extos_hires_time(lua_State *L)
--//--


--[[--
seconds =
extos.monotonic_hires_time()

Returns the number of (SI) seconds since loading the library with high resolution.

--]]--
-- implemented in extos.c as
-- static int extos_monotonic_hires_time(lua_State *L)
--//--

