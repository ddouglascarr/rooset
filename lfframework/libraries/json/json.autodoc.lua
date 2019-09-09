
--[[--
json.null

A special marker to be used to write a value of null to a JSON object or JSON array. The marker is only used for setting a value to null; when reading null values from a JSON object or JSON array via the index operator, via json.get(...), or via pairs(...) or ipairs(...), then the null values will always read as nil.

--]]--
-- implemented as lightuserdata pointing to
-- field "nullmark" of json_lightuserdata struct
-- in json.c
--//--


--[[--
obj =             -- a new JSON object
json.object{
  key1 = value1,  -- key value pair to be set in JSON object
  key2 = value2,  -- another key value pair to be set in JSON object
  ...
}

Converts a Lua table (or any other value with a __pairs metamethod) to a JSON object. The argument is never modified. May also be called without arguments to create an empty JSON object. All JSON objects support iteration using pairs(...). When accessing or iterating over JSON objects, null values will read as nil. To set a value to null, it needs to be set to the special value of json.null though. Setting a value to nil will delete the field.

--]]--
-- implemented as in json.c as
-- static int json_object(lua_State *L)
--//--


--[[--
ary =        -- a new JSON array
json.array{
  value1,    -- first value to be set in JSON array
  value2,    -- second value to be set in JSON array
  ...
}

Converts a Lua table (or any other value with __len and __index metamethods) to a JSON array. The argument is never modified. May also be called without arguments to create an empty JSON array. All JSON arrays support iteration using ipairs(...). When accessing or iterating over JSON arrays, null values will read as nil. To set a value to null, it needs to be set to the special value of json.null though. Setting a value to nil will delete the entry. The length operator (#) returns meaningful results if and only if the set of positive integer keys which have a value assigned (including the special value of null) is equal to the set of all numbers from 1 to some integer n (i.e. the array contains no gaps, but intermediate null values are allowed).

--]]--
-- implemented in json.c as
-- static int json_object(lua_State *L)
--//--


--[[--
value,        -- parsed value/document, or nil in case of error
errmsg =      -- error message if document could not be parsed
json.import(
  str         -- string to be parsed
)

Parses a JSON document. Returns a string, a number, a boolean, json.null, or a JSON object or JSON array as returned by json.object{...} or json.array{...} respectively. The special value json.null is only returned if the top-level value is null; null values within the document always read as nil (see json.null).

--]]--
-- implemented in json.c as
-- static int json_import(lua_State *L)
--//--


--[[--
value =      -- value that has been read, or nil if path does not exist
json.get(
  document,  -- JSON value as returned by json.import(...), json.object{...}, etc., or a Lua table
  key1,      -- first path element (e.g. a string key to descent into an object)
  key2,      -- second path element (e.g. an integer key to descent into an array)
  ...,
  last_key   -- last path element
)

Reads a value from a JSON document by following a given path that may contain string keys (to descent into an object) or integer keys (to descent into an array). A JSON value of null is returned as nil. This function also works on plain Lua tables instead of JSON objects/arrays.

Examples:
json.get(json.import('{"a": {"b": 3}}'), "a", "b") == 3
json.get(json.import('{"a": {"b": 3}}'), "c", "d") == nil
json.get(json.import('{"n": null}'), "n") == nil
json.get({a = {b = 3}}, "a", "b") == 3

--]]--
-- implemented in json.c as
-- static int json_get(lua_State *L)
--//--


--[[--
type_str =   -- "object", "array", "string", "number", "boolean", "null", string "nil", or nil
json.type(
  document,  -- JSON value as returned by json.import(...), json.object{...}, etc., or a Lua table
  key1,      -- first path element (e.g. a string key to descent into an object)
  key2,      -- second path element (e.g. an integer key to descent into an array)
  ...,
  last_key   -- last path element
)

Determines the type of a value in a JSON document by following a given path that may contain string keys (to descent into an object) or integer keys (to descent into an array). If the path but its last path element could be followed, then the string "nil" is returned. If the previous path elements could not be followed, then nil itself (no string) is returned. Otherwise the type of the value is returned, whereas the string "null" indicates a JSON null value. This function also works on plain Lua tables instead of JSON documents.

Examples:
json.type(json.import('{"a": {"b": 3}}'), "a", "b") == 3
json.type(json.import('{"a": {"b": null}}'), "a", "b") == "null"
json.type(json.import('{"a": {"b": null}}'), "a", "c") == "nil"
json.type(json.import('{"a": {"b": null}}'), "d", "c") == nil

--]]--
-- implemented in json.c as
-- static int json_type(lua_State *L)
--//--


--[[--
document =   -- first argument is returned for convenience
json.set(
  document,  -- JSON value as returned by json.import(...), json.object{...}, etc., or a Lua table
  value,     -- new value to be set within the document
  key1,      -- first path element (e.g. a string key to descent into an object)
  key2,      -- second path element (e.g. an integer key to descent into an array)
  ...
  last_key   -- last path element
)

Sets a value in a JSON document by following a given path that may contain string keys (to descent into an object) or integer keys (to descent into an array). If the path does not exist (or contains objects where arrays are expected or vice versa), then the necessary intermediate JSON objects or JSON arrays are created. This function also works on plain Lua tables but will create JSON objects or arrays where necessary.

--]]--
-- implemented in json.c as
-- static int json_set(lua_State *L)
--//--


--[[--
str =          -- encoded JSON document as string
json.export(
  document,    -- JSON value (object, array, string, number, boolean, null), or a Lua table
  indentation  -- optional indentation string for pretty printing, or true for two spaces
)

Encodes a JSON document. Both json.null and nil are accepted to encode the null value on top-level. Since the order of object keys is deterministic, this function may also be used to compare two JSON documents for equality: json.export(a) == json.export(b). If the indentation argument is nil or false, then pretty printing is disabled.

--]]--
-- implemented in json.c as
-- static int json_export(lua_State *L)
--//--

