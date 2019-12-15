#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
/* TODO: stdint.h only needed for Lua 5.2 compatibility */

// maximum number of nested JSON values (objects and arrays):
// NOTE: json_import can store 2^32 / 3 levels on stack swap (using
//       also negative indicies after integer wraparound), and
//       json_export can store even more levels, so 1024^3 =
//       1073741824 is a safe value and allows practically unlimited
//       levels for JSON documents <= 2 GiB.
#define JSON_MAXDEPTH (1024*1024*1024)

// define type JSON_LIGHTUSERDATA and
// generate dummy memory addresses for lightuserdata values:
#define JSON_LIGHTUSERDATA char
static struct {
  JSON_LIGHTUSERDATA nullmark;   // lightuserdata value represents a NULL value
  JSON_LIGHTUSERDATA shadowtbl;  // lightuserdata key for shadow table
} json_lightuserdata;

// macros for special nullmark value:
#define json_isnullmark(L, i) (lua_touserdata((L), (i)) == &json_lightuserdata.nullmark)
#define json_pushnullmark(L) lua_pushlightuserdata((L), &json_lightuserdata.nullmark)

// macros for getting and setting shadow tables
#define json_setshadow(L, i) lua_rawsetp((L), (i), &json_lightuserdata.shadowtbl)
#define json_getshadow(L, i) lua_rawgetp((L), (i), &json_lightuserdata.shadowtbl)
#define json_createproxy(L) lua_createtable((L), 0, 1)

// generate additional dummy memory addresses that represent Lua objects
// via lightuserdata keys and LUA_REGISTRYINDEX:
static struct {
  JSON_LIGHTUSERDATA objectmt;  // metatable for JSON objects
  JSON_LIGHTUSERDATA arraymt;   // metatable for JSON arrays
} json_registry;

// macros for usage of Lua registry:
#define json_regpointer(x) (&json_registry.x)
#define json_regfetchpointer(L, x) lua_rawgetp((L), LUA_REGISTRYINDEX, (x))
#define json_regfetch(L, x) json_regfetchpointer(L, json_regpointer(x))
#define json_regstore(L, x) lua_rawsetp(L, LUA_REGISTRYINDEX, json_regpointer(x))

// returns the string "<JSON null marker>":
static int json_nullmark_tostring(lua_State *L) {
  lua_pushliteral(L, "<JSON null marker>");
  return 1;
}

#define json_object_source_idx 1
#define json_object_iterator_idx 2
#define json_object_output_idx 3
#define json_object_shadow_idx 4
#define json_object_iterfun_idx 5
#define json_object_itertbl_idx 6

// converts a Lua table (or any other iterable value) to a JSON object:
// (does never modify the argument, returns an empty object or array if argument is nil)
static int json_object(lua_State *L) {
  // determine is argument is given:
  if (lua_isnoneornil(L, json_object_source_idx)) {
    // if no argument is given (or if argument is nil),
    // create proxy table with shadow table, and leave proxy table on top of stack:
    json_createproxy(L);
    lua_newtable(L);
    json_setshadow(L, -2);
  } else {
    // if an argument was given,
    // stack shall contain only one function argument:
    lua_settop(L, 1);
    // check if there is an iterator function in its metatable:
    if (luaL_getmetafield(L, json_object_source_idx, "__pairs")) {
      // if there is an iterator function,
      // leave it on stack position 2 and verify its type:
      if (lua_type(L, json_object_iterator_idx) != LUA_TFUNCTION)
        return luaL_error(L, "__pairs metamethod is not a function");
    } else {
      // if there is no iterator function,
      // verify the type of the argument itself:
      luaL_checktype(L, json_object_source_idx, LUA_TTABLE);
      // push nil onto stack position 2:
      lua_pushnil(L);
    }
    // create result table on stack position 3:
    json_createproxy(L);
    // create shadow table on stack position 4:
    lua_newtable(L);
    lua_pushvalue(L, -1);
    json_setshadow(L, -3);
    // check if iterator function exists:
    if (lua_isnil(L, json_object_iterator_idx)) {
      // if there is no iterator function,
      // copy all string key value pairs to shadow table:
      for (lua_pushnil(L); lua_next(L, json_object_source_idx); lua_pop(L, 1)) {
        if (lua_type(L, -2) == LUA_TSTRING) {
          lua_pushvalue(L, -2);
          lua_pushvalue(L, -2);
          lua_rawset(L, json_object_shadow_idx);
        }
      }
    } else {
      // if there is an iterator function,
      // call iterator function with source value (first argument)
      // and store 3 result values on stack positions 5 through 7:
      lua_pushvalue(L, json_object_iterator_idx);
      lua_pushvalue(L, 1);
      lua_call(L, 1, 3);
      // iterate through key value pairs and store some of them in shadow table
      // while replacing nil values with null-marker:
      while (1) {
        // call iterfun function:
        lua_pushvalue(L, json_object_iterfun_idx);
        lua_pushvalue(L, json_object_itertbl_idx);
        lua_pushvalue(L, -3);
        lua_remove(L, -4);
        lua_call(L, 2, 2);
        // break iteration loop if key is nil:
        if (lua_isnil(L, -2)) break;
        // store key value pair only if key type is correct:
        if (lua_type(L, -2) == LUA_TSTRING) {
          // if key type is correct,
          // push key onto stack:
          lua_pushvalue(L, -2);
          // if value is nil, push null-marker onto stack (as value):
          if (lua_isnil(L, -2)) json_pushnullmark(L);
          // else push value onto stack:
          else lua_pushvalue(L, -2);
          // set key value pair in shadow table:
          lua_rawset(L, json_object_shadow_idx);
        }
        // pop value from stack, but leave key on stack:
        lua_pop(L, 1);
      }
    }
    // let result table be on top of stack:
    lua_settop(L, json_object_output_idx);
  }
  // set metatable (for result table on top of stack):
  json_regfetch(L, objectmt);
  lua_setmetatable(L, -2);
  // return table on top of stack:
  return 1;
}

#define json_array_source_idx 1
#define json_array_output_idx 2
#define json_array_shadow_idx 3

// converts a Lua table (or any other iterable value) to a JSON array:
// (does never modify the argument, returns an empty object or array if argument is nil)
static int json_array(lua_State *L) {
  // determine is argument is given:
  if (lua_isnoneornil(L, json_array_source_idx)) {
    // if no argument is given (or if argument is nil),
    // create proxy table with shadow table, and leave proxy table on top of stack:
    json_createproxy(L);
    lua_newtable(L);
    json_setshadow(L, -2);
  } else {
    lua_Integer arrayidx, arraylen;
    // if an argument was given,
    // stack shall contain only one function argument:
    lua_settop(L, 1);
    // create result table on stack position 2:
    json_createproxy(L);
    // create shadow table on stack position 3:
    lua_newtable(L);
    lua_pushvalue(L, -1);
    json_setshadow(L, -3);
    // determine length of array:
    arraylen = luaL_len(L, json_array_source_idx);
    // for an array, copy consecutive integer value pairs to shadow table:
    for (arrayidx=0; arrayidx<arraylen; ) {
      // increment arrayidx at head of loop:
      arrayidx++;
      // get next array entry:
      lua_pushinteger(L, arrayidx);
      lua_gettable(L, json_array_source_idx);
      // check if value is nil:
      if (lua_isnil(L, -1)) {
        // if yes, replace it with null-marker:
        lua_pop(L, 1);
        json_pushnullmark(L);
      }
      // store value in shadow table:
      lua_rawseti(L, json_array_shadow_idx, arrayidx);
    }
    // let result table be on top of stack:
    lua_settop(L, json_array_output_idx);
  }
  // set metatable (for result table on top of stack):
  json_regfetch(L, arraymt);
  lua_setmetatable(L, -2);
  // return table on top of stack:
  return 1;
}

// internal states of JSON parser:
#define JSON_STATE_VALUE 0
#define JSON_STATE_OBJECT_KEY 1
#define JSON_STATE_OBJECT_KEY_TERMINATOR 2
#define JSON_STATE_OBJECT_VALUE 3
#define JSON_STATE_OBJECT_SEPARATOR 4
#define JSON_STATE_ARRAY_VALUE 5
#define JSON_STATE_ARRAY_SEPARATOR 6
#define JSON_STATE_END 7

// special Lua stack indicies for json_import function:
#define json_import_objectmt_idx 2
#define json_import_arraymt_idx 3
#define json_import_stackswap_idx 4

// macros for hex decoding:
#define json_utf16_surrogate(x) ((x) >= 0xD800 && (x) <= 0xDFFF)
#define json_utf16_lead(x) ((x) >= 0xD800 && (x) <= 0xDBFF)
#define json_utf16_tail(x) ((x) >= 0xDC00 && (x) <= 0xDFFF)
#define json_import_readhex(x) \
  do { \
    x = 0; \
    for (i=0; i<4; i++) { \
      x <<= 4; \
      c = str[pos++]; \
      if (c >= '0' && c <= '9') x += c - '0'; \
      else if (c >= 'A' && c <= 'F') x += c - 'A' + 10; \
      else if (c >= 'a' && c <= 'f') x += c - 'a' + 10; \
      else if (c == 0) goto json_import_unexpected_eof; \
      else goto json_import_unexpected_escape; \
    } \
  } while (0)

// decodes a JSON document:
static int json_import(lua_State *L) {
  int stackswapidx = 0;  // elements in stack swap table
  int i;                 // loop variable
  const char *str;       // string to parse
  size_t total;          // total length of string to parse
  size_t pos = 0;        // current position in string to parse
  size_t level = 0;      // nested levels of objects/arrays currently being processed
  int mode = JSON_STATE_VALUE;  // state of parser (i.e. "what's expected next?")
  unsigned char c;       // variable to store a single character to be processed (unsigned!)
  luaL_Buffer luabuf;    // Lua buffer to decode JSON string values
  char *cbuf;            // C buffer to decode JSON string values
  size_t outlen;         // maximum length or write position of C buffer
  long codepoint;        // decoded UTF-16 character or higher codepoint
  long utf16tail;        // second decoded UTF-16 character (surrogate tail)
  size_t arraylen;       // variable to temporarily store the array length
  // require string as argument and convert to C string with length information:
  str = luaL_checklstring(L, 1, &total);
  // if string contains a NULL byte, this is a syntax error
  if (strlen(str) != total) goto json_import_syntax_error;
  // stack shall contain one function argument:
  lua_settop(L, 1);
  // push objectmt onto stack position 2:
  json_regfetch(L, objectmt);
  // push arraymt onto stack position 3:
  json_regfetch(L, arraymt);
  // push table for stack swapping onto stack position 5:
  // (needed to avoid Lua stack overflows)
  lua_newtable(L);
  // main loop of parser:
  json_import_loop:
  // skip whitespace and store next character in variable 'c':
  while (c = str[pos],
    c == ' ' ||
    c == '\f' ||
    c == '\n' ||
    c == '\r' ||
    c == '\t' ||
    c == '\v'
  ) pos++;
  // NOTE: variable c needs to be unsigned in the following code
  // switch statement to handle certain (single) characters:
  switch (c) {
  // handle end of JSON document:
  case 0:
    // if end of JSON document was expected, then return top element of stack as result:
    if (mode == JSON_STATE_END) return 1;
    // otherwise, the JSON document was malformed:
    if (level == 0) {
      lua_pushnil(L);
      lua_pushliteral(L, "Empty string");
    } else {
      json_import_unexpected_eof:
      lua_pushnil(L);
      lua_pushliteral(L, "Unexpected end of JSON document");
    }
    return 2;
  // new JSON object or JSON array:
  case '{':
  case '[':
    // if an encountered JSON object is not expected here, then return an error:
    if (
      c == '{' &&
      mode != JSON_STATE_VALUE &&
      mode != JSON_STATE_OBJECT_VALUE &&
      mode != JSON_STATE_ARRAY_VALUE
    ) goto json_import_syntax_error;
    // if an encountered JSON array is not expected here, then return an error:
    if (
      c == '[' &&
      mode != JSON_STATE_VALUE &&
      mode != JSON_STATE_OBJECT_VALUE &&
      mode != JSON_STATE_ARRAY_VALUE
    ) goto json_import_syntax_error;
    // consume input character:
    pos++;
    // limit nested levels:
    if (level >= JSON_MAXDEPTH) {
      lua_pushnil(L);
      lua_pushfstring(L, "More than %d nested JSON levels", JSON_MAXDEPTH);
      return 2;
    }
    // swap Lua stack entries for previous level to swap table:
    // (avoids depth limitations due to Lua stack size)
    if (level) {
      lua_rawseti(L, json_import_stackswap_idx, ++stackswapidx);
      lua_rawseti(L, json_import_stackswap_idx, ++stackswapidx);
      lua_rawseti(L, json_import_stackswap_idx, ++stackswapidx);
    }
    // increment level:
    level++;
    // create JSON object or JSON array on stack:
    lua_newtable(L);
    // set metatable of JSON object or JSON array:
    lua_pushvalue(L, c == '{' ? json_import_objectmt_idx : json_import_arraymt_idx);
    lua_setmetatable(L, -2);
    // create internal shadow table on stack:
    lua_newtable(L);
    // register internal shadow table:
    lua_pushvalue(L, -1);
    json_setshadow(L, -3);
    // distinguish between JSON objects and JSON arrays:
    if (c == '{') {
      // if JSON object,
      // expect object key (or end of object) to follow:
      mode = JSON_STATE_OBJECT_KEY;
    } else {
      // if JSON array,
      // expect array value (or end of array) to follow:
      mode = JSON_STATE_ARRAY_VALUE;
      // add nil as key (needed to keep stack balance) and as magic to detect arrays:
      if (c == '[') lua_pushnil(L);
    }
    goto json_import_loop;
  // end of JSON object:
  case '}':
    // if end of JSON object is not expected here, then return an error:
    if (
      mode != JSON_STATE_OBJECT_KEY &&
      mode != JSON_STATE_OBJECT_SEPARATOR
    ) goto json_import_syntax_error;
    // jump to common code for end of JSON object and JSON array:
    goto json_import_close;
  // end of JSON array:
  case ']':
    // if end of JSON array is not expected here, then return an error:
    if (
      mode != JSON_STATE_ARRAY_VALUE &&
      mode != JSON_STATE_ARRAY_SEPARATOR
    ) goto json_import_syntax_error;
    // pop nil key/magic (that was needed to keep stack balance):
    lua_pop(L, 1);
    // continue with common code for end of JSON object and JSON array:
  // common code for end of JSON object or JSON array:
  json_import_close:
    // consume input character:
    pos++;
    // pop shadow table:
    lua_pop(L, 1);
    // check if nested:
    if (--level) {
      // if nested,
      // restore previous stack elements from stack swap:
      lua_rawgeti(L, json_import_stackswap_idx, stackswapidx--);
      lua_insert(L, -2);
      lua_rawgeti(L, json_import_stackswap_idx, stackswapidx--);
      lua_insert(L, -2);
      lua_rawgeti(L, json_import_stackswap_idx, stackswapidx--);
      lua_insert(L, -2);
      // check if outer(!) structure is an array or object:
      if (lua_isnil(L, -2)) {
        // select array value processing:
        mode = JSON_STATE_ARRAY_VALUE;
      } else {
        // select object value processing:
        mode = JSON_STATE_OBJECT_VALUE;
      }
      // store value in outer structure:
      goto json_import_process_value;
    }
    // if not nested, then expect end of JSON document and continue with loop:
    mode = JSON_STATE_END;
    goto json_import_loop;
  // key terminator:
  case ':':
    // if key terminator is not expected here, then return an error:
    if (mode != JSON_STATE_OBJECT_KEY_TERMINATOR)
      goto json_import_syntax_error;
    // consume input character:
    pos++;
    // expect object value to follow:
    mode = JSON_STATE_OBJECT_VALUE;
    // continue with loop:
    goto json_import_loop;
  // value terminator (NOTE: trailing comma at end of value or key-value list is tolerated by this parser)
  case ',':
    // branch according to parser state:
    if (mode == JSON_STATE_OBJECT_SEPARATOR) {
      // expect an object key to follow:
      mode = JSON_STATE_OBJECT_KEY;
    } else if (mode == JSON_STATE_ARRAY_SEPARATOR) {
      // expect an array value to follow:
      mode = JSON_STATE_ARRAY_VALUE;
    } else {
       // if value terminator is not expected here, then return an error:
       goto json_import_syntax_error;
    }
    // consume input character:
    pos++;
    // continue with loop:
    goto json_import_loop;
  // string literal:
  case '"':
    // consume quote character:
    pos++;
    // find last character in input string:
    outlen = pos;
    while ((c = str[outlen]) != '"') {
      // consume one character:
      outlen++;
      // handle unexpected end of JSON document:
      if (c == 0) goto json_import_unexpected_eof;
      // consume one extra character when encountering an escaped quote:
      else if (c == '\\' && str[outlen] == '"') outlen++;
    }
    // determine buffer length:
    outlen -= pos;
    // check if string is non empty:
    if (outlen) {
      // prepare buffer to decode string (with maximum possible length) and set write position to zero:
      cbuf = luaL_buffinitsize(L, &luabuf, outlen);
      outlen = 0;
      // loop through the characters until encountering end quote:
      while ((c = str[pos++]) != '"') {
        // NOTE: unexpected end cannot happen anymore
        if (c < 32 || c == 127) {
          // do not allow ASCII control characters:
          // NOTE: illegal UTF-8 sequences and extended control characters are not sanitized
          //       by this parser to allow different encodings than Unicode
          lua_pushnil(L);
          lua_pushliteral(L, "Unexpected control character in JSON string");
          return 2;
        } else if (c == '\\') {
          // read next char after backslash escape:
          c = str[pos++];
          switch (c) {
          // unexpected end-of-string:
          case 0:
            goto json_import_unexpected_eof;
          // unescaping of quotation mark, slash, and backslash:
          case '"':
          case '/':
          case '\\':
            cbuf[outlen++] = c;
            break;
          // unescaping of backspace:
          case 'b': cbuf[outlen++] = '\b'; break;
          // unescaping of form-feed:
          case 'f': cbuf[outlen++] = '\f'; break;
          // unescaping of new-line:
          case 'n': cbuf[outlen++] = '\n'; break;
          // unescaping of carriage-return:
          case 'r': cbuf[outlen++] = '\r'; break;
          // unescaping of tabulator:
          case 't': cbuf[outlen++] = '\t'; break;
          // unescaping of UTF-16 characters
          case 'u':
            // decode 4 hex nibbles:
            json_import_readhex(codepoint);
            // handle surrogate character:
            if (json_utf16_surrogate(codepoint)) {
              // check if first surrogate is in valid range:
              if (json_utf16_lead(codepoint)) {
                // require second surrogate:
                if ((c = str[pos++]) != '\\' || (c = str[pos++]) != 'u') {
                  if (c == 0) goto json_import_unexpected_eof;
                  else goto json_import_wrong_surrogate;
                }
                // read 4 hex nibbles of second surrogate character:
                json_import_readhex(utf16tail);
                // check if second surrogate is in valid range:
                if (!json_utf16_tail(utf16tail)) goto json_import_wrong_surrogate;
                // calculate codepoint:
                codepoint = 0x10000 + (utf16tail - 0xDC00) + (codepoint - 0xD800) * 0x400;
              } else {
                // throw error for wrong surrogates:
                json_import_wrong_surrogate:
                lua_pushnil(L);
                lua_pushliteral(L, "Illegal UTF-16 surrogate in JSON string escape sequence");
                return 2;
              }
            }
            // encode as UTF-8:
            if (codepoint < 0x80) {
              cbuf[outlen++] = (char)codepoint;
            } else if (codepoint < 0x800) {
              cbuf[outlen++] = (char)(0xc0 | (codepoint >> 6));
              cbuf[outlen++] = (char)(0x80 | (codepoint & 0x3f));
            } else if (codepoint < 0x10000) {
              cbuf[outlen++] = (char)(0xe0 | (codepoint >> 12));
              cbuf[outlen++] = (char)(0x80 | ((codepoint >> 6) & 0x3f));
              cbuf[outlen++] = (char)(0x80 | (codepoint & 0x3f));
            } else {
              cbuf[outlen++] = (char)(0xf0 | (codepoint >> 18));
              cbuf[outlen++] = (char)(0x80 | ((codepoint >> 12) & 0x3f));
              cbuf[outlen++] = (char)(0x80 | ((codepoint >> 6) & 0x3f));
              cbuf[outlen++] = (char)(0x80 | (codepoint & 0x3f));
            }
            break;
          // unexpected escape sequence:
          default:
            json_import_unexpected_escape:
            lua_pushnil(L);
            lua_pushliteral(L, "Unexpected string escape sequence in JSON document");
            return 2;
          }
        } else {
          // normal character:
          cbuf[outlen++] = c;
        }
      }
      // process buffer to Lua string:
      luaL_pushresultsize(&luabuf, outlen);
    } else {
      // if JSON string is empty,
      // push empty Lua string:
      lua_pushliteral(L, "");
      // consume closing quote:
      pos++;
    }
    // continue with processing of decoded string:
    goto json_import_process_value;
  }
  // process values whose type is is not deducible from a single character:
  if ((c >= '0' && c <= '9') || c == '-' || c == '+') {
    // try to parse number:
    double numval;
    char *endptr;
    size_t endpos;
    // use strtod() call to parse a (double precision) floating point number
    // and to determine length of number:
    numval = strtod(str+pos, &endptr);
    // catch parsing errors:
    if (endptr == str+pos) goto json_import_syntax_error;
    // calculate end position of number:
    endpos = endptr - str;
#if LUA_VERSION_NUM >= 503
    // try alternative integer interpretation:
    {
      lua_Integer intval = 0;
      size_t curpos;
      if (c >= '0' && c <= '9') intval = c - '0';
      for (curpos=pos+1; curpos<endpos; curpos++) {
        lua_Integer d = str[curpos] - '0';
        if (d < 0 || d > 9) break;
        if (c == '-') {
          // NOTE: rounding of negative integer division may be undefined
          if (
            intval == LUA_MININTEGER ||
            -intval > (-(LUA_MININTEGER+10) - d) / 10 + 1
          ) break;
          intval = 10 * intval - d;
        } else {
          if (intval > (LUA_MAXINTEGER - d) / 10) break;
          intval = 10 * intval + d;
        }
      }
      // push result onto Lua stack:
      if (curpos == endpos) lua_pushinteger(L, intval);
      else lua_pushnumber(L, numval);
    }
#else
    // push result onto Lua stack:
    lua_pushnumber(L, numval);
#endif
    // consume characters that were parsed:
    pos = endpos;
  } else if (!strncmp(str+pos, "true", 4)) {
    // consume 4 input characters for "true":
    pos += 4;
    // put Lua true value onto stack:
    lua_pushboolean(L, 1);
  } else if (!strncmp(str+pos, "false", 5)) {
    // consume 5 input characters for "false":
    pos += 5;
    // put Lua false value onto stack:
    lua_pushboolean(L, 0);
  } else if (!strncmp(str+pos, "null", 4)) {
    // consume 4 input characters for "null":
    pos += 4;
    // push special null-marker onto stack:
    json_pushnullmark(L);
  } else {
    // all other cases are a syntax error:
    goto json_import_syntax_error;
  }
  // process a decoded value or key value pair (expected on top of Lua stack):
  json_import_process_value:
  switch (mode) {
  // an object key has been read:
  case JSON_STATE_OBJECT_KEY:
    // if an object key is not a string, then this is a syntax error:
    if (lua_type(L, -1) != LUA_TSTRING) goto json_import_syntax_error;
    // expect key terminator to follow:
    mode = JSON_STATE_OBJECT_KEY_TERMINATOR;
    // continue with loop:
    goto json_import_loop;
  // a key value pair has been read:
  case JSON_STATE_OBJECT_VALUE:
    // store key value pair in outer shadow table:
    lua_rawset(L, -3);
    // expect value terminator (or end of object) to follow:
    mode = JSON_STATE_OBJECT_SEPARATOR;
    // continue with loop:
    goto json_import_loop;
  // an array value has been read:
  case JSON_STATE_ARRAY_VALUE:
    // get current array length:
    arraylen = lua_rawlen(L, -3);
    // throw error if array would exceed INT_MAX-1 elements:
    // NOTE: Lua 5.3 may support more elements, but C libraries may not
    if (arraylen > INT_MAX-1) {
      lua_pushnil(L);
      lua_pushfstring(L, "Array exceeded length of %d elements", INT_MAX-1);
    }
    // store value in outer shadow table:
    lua_rawseti(L, -3, arraylen + 1);
    // expect value terminator (or end of object) to follow:
    mode = JSON_STATE_ARRAY_SEPARATOR;
    // continue with loop
    goto json_import_loop;
  // a single value has been read:
  case JSON_STATE_VALUE:
    // leave value on top of stack, expect end of JSON document, and continue with loop:
    mode = JSON_STATE_END;
    goto json_import_loop;
  }
  // syntax error handling (reachable by goto statement):
  json_import_syntax_error:
  lua_pushnil(L);
  lua_pushliteral(L, "Syntax error in JSON document");
  return 2;
}

// gets a value or its type from a JSON document (passed as first argument)
// using a path (passed as variable number of keys after the first argument):
static int json_path(lua_State *L, int type_mode) {
  int stacktop;  // number of arguments
  int idx = 2;   // stack index of current argument to process
  // require at least one argument:
  luaL_checkany(L, 1);
  // store stack index of top of stack (number of arguments):
  stacktop = lua_gettop(L);
  // use first argument as "current value" (stored on top of stack):
  lua_pushvalue(L, 1);
  // process each "path key" (2nd argument and following arguments):
  while (idx <= stacktop) {
    // if "current value" (on top of stack) is nil, then the path cannot be walked and nil is returned:
    if (lua_isnil(L, -1)) return 1;
    // try to get shadow table of "current value":
    json_getshadow(L, -1);
    if (lua_isnil(L, -1)) {
      // if no shadow table is found,
      if (lua_type(L, -2) == LUA_TTABLE) {
        // and if "current value" is a table,
        // pop nil from stack:
        lua_pop(L, 1);
        // get "next value" using the "path key":
        lua_pushvalue(L, idx++);
        lua_gettable(L, -2);
      } else {
        // if "current value" is not a table,
        // then the path cannot be walked and nil (already on top of stack) is returned:
        return 1;
      }
    } else {
      // if a shadow table is found,
      // set "current value" to its shadow table:
      lua_replace(L, -2);
      // get "next value" using the "path key":
      lua_pushvalue(L, idx++);
      lua_rawget(L, -2);
    }
    // the "next value" replaces the "current value":
    lua_replace(L, -2);
  }
  if (!type_mode) {
    // if a value (and not its type) was requested,
    // check if value is the null-marker, and store nil on top of Lua stack in that case:
    if (json_isnullmark(L, -1)) lua_pushnil(L);
  } else {
    // if the type was requested,
    // check if value is the null-marker:
    if (json_isnullmark(L, -1)) {
      // if yes, store string "null" on top of Lua stack:
      lua_pushliteral(L, "null");
    } else {
      // otherwise,
      // check if metatable indicates "object" or "array":
      if (lua_getmetatable(L, -1)) {
        json_regfetch(L, objectmt);
        if (lua_rawequal(L, -2, -1)) {
          // if value has metatable for JSON objects,
          // return string "object":
          lua_pushliteral(L, "object");
          return 1;
        }
        json_regfetch(L, arraymt);
        if (lua_rawequal(L, -3, -1)) {
          // if value has metatable for JSON arrays,
          // return string "object":
          lua_pushliteral(L, "array");
          return 1;
        }
        // remove 3 metatables (one of the value, two for comparison) from stack:
        lua_pop(L, 3);
      }
      // otherwise, get the Lua type:
      lua_pushstring(L, lua_typename(L, lua_type(L, -1)));
    }
  }
  // return the top most value on the Lua stack:
  return 1;
}

// gets a value from a JSON document (passed as first argument)
// using a path (passed as variable number of keys after the first argument):
static int json_get(lua_State *L) {
  return json_path(L, 0);
}

// gets a value's type from a JSON document (passed as first argument)
// using a path (passed as variable number of keys after first the argument):
static int json_type(lua_State *L) {
  return json_path(L, 1);
}

// special Lua stack indicies for json_set function:
#define json_set_objectmt_idx 1
#define json_set_arraymt_idx 2

// stack offset of arguments to json_set function:
#define json_set_idxshift 2

// sets a value (passed as second argument) in a JSON document (passed as first argument)
// using a path (passed as variable number of keys starting at third argument):
static int json_set(lua_State *L) {
  int stacktop;  // stack index of top of stack (after shifting)
  int idx;       // stack index of current argument to process
  // require at least three arguments:
  luaL_checkany(L, 1);
  luaL_checkany(L, 2);
  luaL_checkany(L, 3);
  // insert objectmt into stack at position 1 (shifting the arguments):
  json_regfetch(L, objectmt);
  lua_insert(L, 1);
  // insert arraymt into stack at position 2 (shifting the arguments):
  json_regfetch(L, arraymt);
  lua_insert(L, 2);
  // store stack index of top of stack:
  stacktop = lua_gettop(L);
  // use nil as initial "parent value":
  lua_pushnil(L);
  // use first argument as "current value":
  lua_pushvalue(L, 1 + json_set_idxshift);
  // set all necessary values in path:
  for (idx = 3 + json_set_idxshift; idx<=stacktop; idx++) {
    // push metatable of "current value" onto stack:
    if (!lua_getmetatable(L, -1)) lua_pushnil(L);
    // distinguish according to type of path key:
    switch (lua_type(L, idx)) {
    case LUA_TSTRING:
      // if path key is a string,
      // check if "current value" is a JSON object (or table without metatable):
      if (
        lua_rawequal(L, -1, json_set_objectmt_idx) ||
        (lua_isnil(L, -1) && lua_type(L, -2) == LUA_TTABLE)
      ) {
        // if "current value" is acceptable,
        // pop metatable and leave "current value" on top of stack:
        lua_pop(L, 1);
      } else {
        // if "current value" is not acceptable:
        // pop metatable and "current value":
        lua_pop(L, 2);
        // throw error if parent element does not exist:
        if (lua_isnil(L, -1)) return luaL_error(L, "Root element is not a JSON object");
        // push new JSON object as "current value" onto stack:
        json_createproxy(L);
        // create and register shadow table:
        lua_newtable(L);
        json_setshadow(L, -2);
        // set metatable of JSON object: 
        lua_pushvalue(L, json_set_objectmt_idx);
        lua_setmetatable(L, -2);
        // set entry in "parent value":
        lua_pushvalue(L, idx-1);
        lua_pushvalue(L, -2);
        lua_settable(L, -4);
      }
      break;
    case LUA_TNUMBER:
      // if path key is a number,
      // check if "current value" is a JSON array (or table without metatable):
      if (
        lua_rawequal(L, -1, json_set_arraymt_idx) ||
        (lua_isnil(L, -1) && lua_type(L, -2) == LUA_TTABLE)
      ) {
        // if "current value" is acceptable,
        // pop metatable and leave "current value" on top of stack:
        lua_pop(L, 1);
      } else {
        // if "current value" is not acceptable:
        // pop metatable and "current value":
        lua_pop(L, 2);
        // throw error if parent element does not exist:
        if (lua_isnil(L, -1)) return luaL_error(L, "Root element is not a JSON array");
        // push new JSON array as "current value" onto stack:
        json_createproxy(L);
        // create and register shadow table:
        lua_newtable(L);
        json_setshadow(L, -2);
        // set metatable of JSON array: 
        lua_pushvalue(L, json_set_arraymt_idx);
        lua_setmetatable(L, -2);
        // set entry in "parent value":
        lua_pushvalue(L, idx-1);
        lua_pushvalue(L, -2);
        lua_settable(L, -4);
      }
      break;
    default:
      return luaL_error(L, "Invalid path key of type %s", lua_typename(L, lua_type(L, idx)));
    }
    // check if last path element is being processed:
    if (idx == stacktop) {
      // if the last path element is being processed,
      // set last path value in "current value" container:
      lua_pushvalue(L, idx);
      lua_pushvalue(L, 2 + json_set_idxshift);
      lua_settable(L, -3);
    } else {
      // if the processed path element is not the last,
      // use old "current value" as new "parent value"
      lua_remove(L, -2);
      // push new "current value" onto stack by performing a lookup:
      lua_pushvalue(L, idx);
      lua_gettable(L, -2);
    }
  }
  // return first argument for convenience:
  lua_settop(L, 1 + json_set_idxshift);
  return 1;
}

// returns the length of a JSON array (or zero for a table without numeric keys):
static int json_len(lua_State *L) {
  // require table as first argument:
  luaL_checktype(L, 1, LUA_TTABLE);
  // stack shall contain one function argument:
  lua_settop(L, 1);
  // push shadow table or nil onto stack:
  json_getshadow(L, 1);
  // pop nil from stack if no shadow table has been found:
  if (lua_isnil(L, -1)) lua_pop(L, 1);
  // return length of argument or shadow table:
  lua_pushnumber(L, lua_rawlen(L, -1));
  return 1;
}

// __index metamethod for JSON objects and JSON arrays:
static int json_index(lua_State *L) {
  // require table as first argument:
  luaL_checktype(L, 1, LUA_TTABLE);
  // stack shall contain two function arguments:
  lua_settop(L, 2);
  // replace first argument with its shadow table
  // or throw error if no shadow table is found:
  json_getshadow(L, 1);
  if (lua_isnil(L, -1)) return luaL_error(L, "Shadow table not found");
  lua_replace(L, 1);
  // use key passed as second argument to lookup value in shadow table:
  lua_rawget(L, 1);
  // if value is null-marker, then push nil onto stack:
  if (json_isnullmark(L, 2)) lua_pushnil(L);
  // return either looked up value, or nil
  return 1;
}

// __newindex metamethod for JSON objects and JSON arrays:
static int json_newindex(lua_State *L) {
  // require table as first argument
  luaL_checktype(L, 1, LUA_TTABLE);
  // stack shall contain three function arguments:
  lua_settop(L, 3);
  // replace first argument with its shadow table
  // or throw error if no shadow table is found:
  json_getshadow(L, 1);
  if (lua_isnil(L, -1)) return luaL_error(L, "Shadow table not found");
  lua_replace(L, 1);
  // second and third argument to write to shadow table:
  lua_rawset(L, 1);
  // return nothing:
  return 0;
}

// function returned as first value by json_pairs function:
static int json_pairs_iterfunc(lua_State *L) {
  // require table as first argument
  luaL_checktype(L, 1, LUA_TTABLE);
  // stack shall contain two function arguments:
  lua_settop(L, 2);
  // get next key value pair from shadow table (argument 1) using previous key (argument 2)
  // and return nothing if there is no next pair:
  if (!lua_next(L, 1)) return 0;
  // replace null-marker with nil:
  if (json_isnullmark(L, -1)) {
    lua_pop(L, 1);
    lua_pushnil(L);
  }
  // return key and value (or key and nil, if null-marker was found):
  return 2;
}

// returns a triple such that 'for key, value in pairs(obj) do ... end'
// iterates through all key value pairs (including JSON null values represented as Lua nil):
static int json_pairs(lua_State *L) {
  // require table as first argument
  luaL_checktype(L, 1, LUA_TTABLE);
  // return triple of function json_pairs_iterfunc, shadow table of first argument, and nil:
  lua_pushcfunction(L, json_pairs_iterfunc);
  json_getshadow(L, 1);
  if (lua_isnil(L, -1)) return luaL_error(L, "Shadow table not found");
  lua_pushnil(L);
  return 3;
}

// function returned as first value by json_ipairs function:
static int json_ipairs_iterfunc(lua_State *L) {
  lua_Integer idx;
  // require table as first argument
  luaL_checktype(L, 1, LUA_TTABLE);
  // stack shall contain two function arguments:
  lua_settop(L, 2);
  // calculate new index by incrementing second argument:
  idx = lua_tointeger(L, 2) + 1;
  // do integer lookup in shadow table and store result on stack position 3:
  lua_rawgeti(L, 1, idx);
  // return nothing if there was no value:
  if (lua_isnil(L, 3)) return 0;
  // return new index and
  // either the looked up value if it is not equal to the null-marker
  // or nil instead of null-marker:
  lua_pushinteger(L, idx);
  if (json_isnullmark(L, 3)) lua_pushnil(L);
  else lua_pushvalue(L, 3);
  return 2;
}

// returns a triple such that 'for idx, value in ipairs(ary) do ... end'
// iterates through all values (including JSON null values represented as Lua nil):
static int json_ipairs(lua_State *L) {
  // require table as first argument
  luaL_checktype(L, 1, LUA_TTABLE);
  // return triple of function json_ipairs_iterfunc, shadow table of first argument, and zero:
  lua_pushcfunction(L, json_ipairs_iterfunc);
  json_getshadow(L, 1);
  if (lua_isnil(L, -1)) return luaL_error(L, "Shadow table not found");
  lua_pushinteger(L, 0);
  return 3;
}

// datatype representing a table key:
// (used for sorting)
typedef struct {
  size_t length;
  const char *data;
} json_key_t;

// comparation function for table keys to be passed to qsort function:
static int json_key_cmp(json_key_t *key1, json_key_t *key2) {
  size_t pos = 0;
  unsigned char c1, c2;
  while (1) {
    if (key1->length > pos) {
      if (key2->length > pos) {
        c1 = key1->data[pos];
        c2 = key2->data[pos];
        if (c1 < c2) return -1;
        else if (c1 > c2) return 1;
      } else {
        return 1;
      }
    } else {
      if (key2->length > pos) {
        return -1;
      } else {
        return 0;
      }
    }
    pos++;
  }
}

// constants for type detection of ambiguous tables:
#define JSON_TABLETYPE_UNKNOWN 0
#define JSON_TABLETYPE_OBJECT 1
#define JSON_TABLETYPE_ARRAY 2

typedef struct {
  int type;
  int pos;
  int count;
  json_key_t keys[1];  // or more
} json_container_t;

// special Lua stack indicies for json_export function:
#define json_export_value_idx 1
#define json_export_indentstring_idx 2
#define json_export_objectmt_idx 3
#define json_export_arraymt_idx 4
#define json_export_stackswap_idx 5
#define json_export_luacontainer_idx 6
#define json_export_ccontainer_idx 7
#define json_export_buffer_idx 8

// encodes a JSON document (passed as first argument)
// optionally using indentation (indentation string or true passed as second argument)
static int json_export(lua_State *L) {
  int pretty;           // pretty printing on? (i.e. printing with indentation)
  luaL_Buffer buf;      // Lua buffer containing result string
  lua_Number num;       // number to encode
  char numstr[80];      // encoded number
                        // (21 chars needed for sign, zero, point, 17 significant digits, and NULL byte)
                        // (21 chars needed for sign, 19 digits INT64, and NULL byte)
                        // (80 chars needed for sign, 78 digits INT256, and NULL byte)
                        // (NOTE: we don't know the size of intmax_t and thus use 80)
  const char *str;      // string to encode
  size_t strlen;        // length of string to encode
  size_t strpos ;       // position in string or position of current key
  unsigned char c;      // character to encode (unsigned!)
  char hexcode[7];      // store for unicode hex escape sequence
                        // NOTE: 7 bytes due to backslash, character 'u', 4 hex digits, and terminating NULL byte
  int tabletype;        // table type: unknown, JSON object, or JSON array
  size_t keycount = 0;  // number of string keys in object
  json_key_t *key;      // pointer to C structure containing a string key
  int level = 0;        // current depth level
  int i;                // iteration variable for level dependent repetitions
  int stackswapidx = 0; // elements in stack swap table
  int containerkey = 0; // temporarily set to 1, if a container key is being encoded
  json_container_t *container = NULL; // pointer to current C struct for container information
  // stack shall contain two function arguments:
  lua_settop(L, 2);
  // check if pretty printing (with indentation) is desired:
  if (lua_toboolean(L, json_export_indentstring_idx)) {
    // if yes,
    // set pretty variable to 1:
    pretty = 1;
    // check if second argument is a boolean (true):
    if (lua_isboolean(L, json_export_indentstring_idx)) {
      // if yes,
      // use default indentation if indentation argument is boolean true:
      lua_pushliteral(L, "  ");
      lua_replace(L, json_export_indentstring_idx);
    } else {
      // if no,
      // require second argument to be a string:
      luaL_checktype(L, json_export_indentstring_idx, LUA_TSTRING);
    }
  } else {
    // if no,
    // set pretty variable to 0:
    pretty = 0;
  }
  // push objectmt onto stack position 3:
  json_regfetch(L, objectmt);
  // push arraymt onto stack position 4:
  json_regfetch(L, arraymt);
  // push table for stack swapping onto stack position 5:
  lua_newtable(L);
  // create placeholders on stack positions 6 through 7:
  lua_settop(L, json_export_buffer_idx);
  // create Lua string buffer:
  luaL_buffinit(L, &buf);
  // loop:
  while (1) {
    // if value to encode is the null-marker, then treat it the same as nil:
    if (json_isnullmark(L, json_export_value_idx)) {
      lua_pushnil(L);
      lua_replace(L, json_export_value_idx);
    }
    // distinguish between different Lua types:
    switch (lua_type(L, json_export_value_idx)) {
    // value to encode is nil:
    case LUA_TNIL:
      // add string "null" to output buffer:
      luaL_addstring(&buf, "null");
      break;
    // value to encode is of type number:
    case LUA_TNUMBER:
#if LUA_VERSION_NUM >= 503
      // handle integers:
      if (lua_isinteger(L, json_export_value_idx)) {
        sprintf(numstr, "%ji", (intmax_t)lua_tointeger(L, json_export_value_idx));
        luaL_addstring(&buf, numstr);
        break;
      }
#endif
      // convert value to double precision number:
      num = lua_tonumber(L, json_export_value_idx);
      // throw error if number is not-a-number:
      if (isnan(num)) return luaL_error(L, "JSON export not possible for NaN value");
      // throw error if number is positive or negative infinity:
      if (isinf(num)) return luaL_error(L, "JSON export not possible for infinite numbers");
      // check if float is integral:
      if ((double)trunc((double)num) == (double)num) {
        // use maximum precision:
        sprintf(numstr, "%.17g", num);  // NOTE: e.g. 12345678901234560
      } else {
        // determine necessary precision to represent double precision floating point number:
        sprintf(numstr, "%.15g", num);  // NOTE: e.g. 0.009 should not be 0.008999999999999999
        if (strtod(numstr, NULL) != num) sprintf(numstr, "%.16g", num);
        if (strtod(numstr, NULL) != num) sprintf(numstr, "%.17g", num);
      }
      // add string encoding of the number to the output buffer:
      luaL_addstring(&buf, numstr);
#if LUA_VERSION_NUM >= 503
      // enforce trailing ".0" for floats unless exponential notation was chosen:
      {
        char *p;
        if (numstr[0] == '-' || numstr[0] == '+') p = numstr+1;
        else p = numstr;
        for (; *p; p++) if (*p < '0' || *p > '9') break;
        if (!*p) luaL_addstring(&buf, ".0");
      }
#endif
      break;
    // value to encode is of type boolean:
    case LUA_TBOOLEAN:
      // add string "true" or "false" according to boolean value:
      luaL_addstring(&buf, lua_toboolean(L, json_export_value_idx) ? "true" : "false");
      break;
    // value to encode is of type string:
    case LUA_TSTRING:
      // add quoted and escaped string to output buffer:
      str = lua_tolstring(L, json_export_value_idx, &strlen);
      luaL_addchar(&buf, '"');
      strpos = 0;
      while (strpos < strlen) {
        c = str[strpos++];
        if (c == '"')       luaL_addstring(&buf, "\\\"");
        else if (c == '\\') luaL_addstring(&buf, "\\\\");
        else if (c == 127)  luaL_addstring(&buf, "\\u007F");
        else if (c >= 32)   luaL_addchar(&buf, c);
        else if (c == '\b') luaL_addstring(&buf, "\\b");
        else if (c == '\f') luaL_addstring(&buf, "\\f");
        else if (c == '\n') luaL_addstring(&buf, "\\n");
        else if (c == '\r') luaL_addstring(&buf, "\\r");
        else if (c == '\t') luaL_addstring(&buf, "\\t");
        else if (c == '\v') luaL_addstring(&buf, "\\v");
        else {
          sprintf(hexcode, "\\u%04X", c);
          luaL_addstring(&buf, hexcode);
        }
      }
      luaL_addchar(&buf, '"');
      break;
    // value to encode is of type table (this includes JSON objects and JSON arrays):
    case LUA_TTABLE:
      // use table's metatable to try to determine type of table:
      tabletype = JSON_TABLETYPE_UNKNOWN;
      if (lua_getmetatable(L, json_export_value_idx)) {
        if (lua_rawequal(L, -1, json_export_objectmt_idx)) {
          tabletype = JSON_TABLETYPE_OBJECT;
        } else {
          if (lua_rawequal(L, -1, json_export_arraymt_idx)) {
            tabletype = JSON_TABLETYPE_ARRAY;
          } else {
            return luaL_error(L, "JSON export not possible for tables with nonsupported metatable");
          }
        }
        // reset stack (pop metatable from stack):
        lua_pop(L, 1);
      }
      // replace table with its shadow table if existent:
      json_getshadow(L, json_export_value_idx);
      if (lua_isnil(L, -1)) lua_pop(L, 1);
      else lua_replace(L, json_export_value_idx); 
      // check if type of table is still undetermined
      // and optionally calculate number of string keys (keycount)
      // or set keycount to zero:
      keycount = 0;
      if (tabletype == JSON_TABLETYPE_UNKNOWN) {
        // if type of table is undetermined,
        // iterate over all keys:
        for (lua_pushnil(L); lua_next(L, json_export_value_idx); lua_pop(L, 1)) {
          switch (lua_type(L, -2)) {
          case LUA_TSTRING:
            // for string keys,
            // increase keycount (may avoid another iteration):
            keycount++;
            // if type of table was unknown, then type of table is a JSON object now:
            if (tabletype == JSON_TABLETYPE_UNKNOWN) tabletype = JSON_TABLETYPE_OBJECT;
            // if type of table was a JSON array, then the type of table is ambiguous now
            // and an error is thrown:
            else if (tabletype == JSON_TABLETYPE_ARRAY) goto json_export_tabletype_error;
            break;
          case LUA_TNUMBER:
            // for numeric keys,
            // if type of table was unknown, then type of table is a JSON array now:
            if (tabletype == JSON_TABLETYPE_UNKNOWN) tabletype = JSON_TABLETYPE_ARRAY;
            // if type of table was a JSON object, then the type of table is ambiguous now
            // and an error is thrown:
            else if (tabletype == JSON_TABLETYPE_OBJECT) goto json_export_tabletype_error;
            break;
          }
        }
      }
      // raise error if too many nested levels:
      if (level >= JSON_MAXDEPTH) {
        return luaL_error(L, "More than %d nested JSON levels", JSON_MAXDEPTH);
      }
      // store previous container information (if existent) on stack swap
      // and increase level variable:
      if (level++) {
        lua_pushvalue(L, json_export_luacontainer_idx);
        lua_rawseti(L, json_export_stackswap_idx, ++stackswapidx);
        lua_pushvalue(L, json_export_ccontainer_idx);
        lua_rawseti(L, json_export_stackswap_idx, ++stackswapidx);
      }
      // use value as current container:
      lua_pushvalue(L, json_export_value_idx);
      lua_replace(L, json_export_luacontainer_idx);
      // distinguish between JSON objects and JSON arrays:
      switch (tabletype) {
      // JSON object:
      case JSON_TABLETYPE_OBJECT:
        // calculate count of string keys unless it has been calculated before:
        if (!keycount) {
          for (lua_pushnil(L); lua_next(L, json_export_luacontainer_idx); lua_pop(L, 1)) {
            if (lua_type(L, -2) == LUA_TSTRING) keycount++;
          }
        }
        // allocate memory for C structure containing string keys and container iteration state:
        container = lua_newuserdata(L, sizeof(json_container_t) + (keycount-1) * sizeof(json_key_t));
        // store reference to C structure on designated stack position:
        lua_replace(L, json_export_ccontainer_idx);
        // initialize C structure for container state:
        container->type = JSON_TABLETYPE_OBJECT;
        container->count = keycount;
        container->pos = 0;
        // check if object contains any keys:
        if (keycount) {
          // if yes,
          // copy all string keys to the C structure (and reset container->pos again):
          for (lua_pushnil(L); lua_next(L, json_export_luacontainer_idx); lua_pop(L, 1)) {
            if (lua_type(L, -2) == LUA_TSTRING) {
              json_key_t *key = &container->keys[container->pos++];
              key->data = lua_tolstring(L, -2, &key->length);
            }
          }
          container->pos = 0;
          // sort C array using quicksort:
          qsort(container->keys, keycount, sizeof(json_key_t), (void *)json_key_cmp);
        }
        // add opening bracket to output buffer:
        luaL_addchar(&buf, '{');
        break;
      // JSON array:
      case JSON_TABLETYPE_ARRAY:
        // allocate memory for C structure for container iteration state:
        container = lua_newuserdata(L, sizeof(json_container_t) - sizeof(json_key_t));
        // store reference to C structure on designated stack position:
        lua_replace(L, json_export_ccontainer_idx);
        // initialize C structure for container state:
        container->type = JSON_TABLETYPE_ARRAY;
        container->pos = 0;
        // add opening bracket to output buffer:
        luaL_addchar(&buf, '[');
        break;
      default:
        // throw error if table type is unknown:
        json_export_tabletype_error:
        return luaL_error(L, "JSON export not possible for ambiguous table (cannot decide whether it is an object or array)");
      }
      break;
    default:
    // all other datatypes are considered an error:
    return luaL_error(L, "JSON export not possible for values of type \"%s\"", lua_typename(L, lua_type(L, json_export_value_idx)));
    }
    // check if a container is being processed:
    if (container) {
      // if yes,
      // execute code for container iteration:
      json_export_container:
      // distinguish between JSON objects and JSON arrays:
      switch (container->type) {
      // JSON object:
      case JSON_TABLETYPE_OBJECT:
        // finish iteration if all string keys have been processed:
        if (container->pos == container->count) goto json_export_close;
        // check if the key has already been exported:
        if (!containerkey) {
          // if no,
          // add a comma to the output buffer if necessary:
          if (container->pos) luaL_addchar(&buf, ',');
          // push current string key on top of stack:
          key = &container->keys[container->pos];
          lua_pushlstring(L, key->data, key->length);
          // set containerkey variable to true:
          containerkey = 1;
        } else {
          // if a key has already been exported,
          // add a colon to the output buffer:
          luaL_addchar(&buf, ':');
          // add a space to the output buffer for pretty results:
          if (pretty) luaL_addchar(&buf, ' ');
          // push current string key on top of stack:
          key = &container->keys[container->pos];
          lua_pushlstring(L, key->data, key->length);
          // replace string key on top of stack with corresponding value:
          lua_rawget(L, json_export_luacontainer_idx);
          // reset containerkey variable
          containerkey = 0;
          // increase number of processed key value pairs:
          container->pos++;
        }
        // store key or value on top of stack in designated stack position:
        lua_replace(L, json_export_value_idx);
        break;
      // JSON array:
      case JSON_TABLETYPE_ARRAY:
        // store next value in designated stack position:
        lua_rawgeti(L, json_export_luacontainer_idx, container->pos+1);
        lua_replace(L, json_export_value_idx);
        // finish iteration if value is nil:
        if (lua_isnil(L, json_export_value_idx)) goto json_export_close;
        // add a comma to the output buffer if necessary:
        if (container->pos) luaL_addchar(&buf, ',');
        // increase number of processed values:
        container->pos++;
        break;
      // common code for closing JSON objects or JSON arrays:
      json_export_close:
        // decrement level variable:
        level--;
        // handle indentation for pretty results:
        if (pretty && container->pos) {
          luaL_addchar(&buf, '\n');
          for (i=0; i<level; i++) {
            lua_pushvalue(L, json_export_indentstring_idx);
            luaL_addvalue(&buf);
          }
        }
        // add closing bracket to output buffer:
        luaL_addchar(&buf, container->type == JSON_TABLETYPE_OBJECT ? '}' : ']');
        // finish export if last level has been closed:
        if (!level) goto json_export_finish;
        // otherwise,
        // recall previous container information from stack swap
        // and set C pointer to corresponding C struct:
        lua_rawgeti(L, json_export_stackswap_idx, stackswapidx--);
        lua_replace(L, json_export_ccontainer_idx);
        container = lua_touserdata(L, json_export_ccontainer_idx);
        lua_rawgeti(L, json_export_stackswap_idx, stackswapidx--);
        lua_replace(L, json_export_luacontainer_idx);
        // repeat code for container iteration:
        goto json_export_container;
      }
      // handle indentation for pretty results:
      if (pretty && (containerkey || container->type == JSON_TABLETYPE_ARRAY)) {
        luaL_addchar(&buf, '\n');
        for (i=0; i<level; i++) {
          lua_pushvalue(L, json_export_indentstring_idx);
          luaL_addvalue(&buf);
        }
      }
    } else {
      // if no container is being processed,
      // finish export:
      json_export_finish:
      // for pretty results, add final newline character if outermost container is processed:
      if (pretty) luaL_addchar(&buf, '\n');
      // create and return Lua string from buffer contents
      luaL_pushresult(&buf);
      return 1;
    }
  }
}

// functions in library module:
static const struct luaL_Reg json_module_functions[] = {
  {"object", json_object},
  {"array",  json_array},
  {"import", json_import},
  {"export", json_export},
  {"get",    json_get},
  {"type",   json_type},
  {"set",    json_set},
  {NULL, NULL}
};

// metamethods for JSON objects, JSON arrays, and unknown JSON collections (object or array):
static const struct luaL_Reg json_metatable_functions[] = {
  {"__len", json_len},
  {"__index", json_index},
  {"__newindex", json_newindex},
  {"__pairs", json_pairs},
  {"__ipairs", json_ipairs},
  {"__tostring", json_export},
  {NULL, NULL}
};

// metamethods for JSON null marker:
static const struct luaL_Reg json_nullmark_metamethods[] = {
  {"__tostring", json_nullmark_tostring},
  {NULL, NULL}
};

// initializes json library:
int luaopen_json(lua_State *L) {
  // empty stack:
  lua_settop(L, 0);
  // push library module onto stack position 1:
  lua_newtable(L);
  // register library functions:
  luaL_setfuncs(L, json_module_functions, 0);
  // create and store objectmt:
  lua_newtable(L);
  luaL_setfuncs(L, json_metatable_functions, 0);
  json_regstore(L, objectmt);
  // create and store arraymt:
  lua_newtable(L);
  luaL_setfuncs(L, json_metatable_functions, 0);
  json_regstore(L, arraymt);
  // set metatable of null marker and make it available through library module:
  json_pushnullmark(L);
  lua_newtable(L);
  luaL_setfuncs(L, json_nullmark_metamethods, 0);
  lua_setmetatable(L, -2);
  lua_setfield(L, 1, "null");
  // return library module (that's expected on top of stack):
  return 1;
}
