/*
 * minimalistic Lua LDAP interface library
 *
 * The library does not set any global variable itself and must thus be
 * loaded with
 *
 *     mldap = require("mldap")
 *
 * or a similar statement.
 *
 * The library provides two functions, conn = bind{...} and unbind(conn)
 * to connect to and disconnect from the LDAP server respectively. The
 * unbind function is also provided as a method of the connection userdata
 * object (see below).
 *
 * The arguments to the bind{...} function are documented in the source code
 * (see C function mldap_bind). In case of success, the bind function returns
 * a userdata object that provides two methods: query{...} and unbind(). In
 * case of error, the bind function returns nil as first return value, an
 * error string as second return value, and a numeric error code as third
 * return value. A positive error code is an LDAP resultCode, a negative
 * error code is an OpenLDAP API error code:
 *
 *     connection, error_string, numeric_error_code = mldap.bind{...}
 *
 * For translating numeric error codes to an identifying (machine readable)
 * string identifier, the library provides in addition to the two functions
 * a table named 'errorcodes', for example:
 *
 *     49 == mldap.errorcodes["invalid_credentials"]
 *
 * and
 *
 *     mldap.errorcodes[49] == "invalid_credentials"
 *
 * The arguments and return value of the query{...} method of the connection
 * userdata object are also documented in the source code below (see
 * C function mldap_query). Error conditions are reported the same way as the
 * bind{...} function does.
 *
 * To close the connection, either the unbind() function of the library or
 * the unbind() method can be called; it is allowed to call them multiple
 * times, and they are also invoked by the garbage collector.
 *
 */

// Lua header inclusions:
#include <lua.h>
#include <lauxlib.h>

// OpenLDAP header inclusions:
#include <ldap.h>

// Standard C inclusions:
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// Error code translation is included from separate C file:
#include "mldap_errorcodes.c"

// Provide compatibility with Lua 5.1:
#if LUA_VERSION_NUM < 502
#define luaL_newlib(L, t) lua_newtable((L)); luaL_register(L, NULL, t)
#define lua_rawlen lua_objlen
#define lua_len lua_objlen
#define luaL_setmetatable(L, regkey) \
  lua_getfield((L), LUA_REGISTRYINDEX, (regkey)); \
  lua_setmetatable((L), -2);
#endif

// prefix for all Lua registry entries of this library:
#define MLDAP_REGKEY "556aeaf3c864af2e_mldap_"


static const char *mldap_get_named_string_arg(
  // gets a named argument of type "string" from a table at the given stack position

  lua_State *L,         // pointer to lua_State variable
  int idx,              // stack index of the table containing the named arguments
  const char *argname,  // name of the argument
  int mandatory         // if not 0, then the argument is mandatory and an error is raised if it isn't found

  // leaves the string as new element on top of the stack
) {

  // pushes the table entry with the given argument name on top of the stack:
  lua_getfield(L, idx, argname);

  // check, if the entry is nil or false:
  if (!lua_toboolean(L, -1)) {

    // throw error, if named argument is mandatory:
    if (mandatory) return luaL_error(L, "Named argument '%s' missing", argname), NULL;

    // return NULL pointer, if named argument is not mandatory:
    return NULL;

  }

  // throw error, if the value of the argument is not string:
  if (lua_type(L, -1) != LUA_TSTRING) return luaL_error(L, "Named argument '%s' is not a string", argname), NULL;

  // return a pointer to the string, leaving the string on top of the stack to avoid garbage collection:
  return lua_tostring(L, -1);

  // leaves one element on the stack
}


static int mldap_get_named_number_arg(
  // gets a named argument of type "number" from a table at the given stack position

  lua_State *L,             // pointer to lua_State variable
  int idx,                  // stack index of the table containing the named arguments
  const char *argname,      // name of the argument
  int mandatory,            // if not 0, then the argument is mandatory and an error is raised if it isn't found
  lua_Number default_value  // default value to return, if the argument is not mandatory and nil or false

  // opposed to 'mldap_get_named_string_arg', this function leaves no element on the stack
) {

  lua_Number value;  // value to return

  // pushes the table entry with the given argument name on top of the stack:
  lua_getfield(L, idx, argname);

  // check, if the entry is nil or false:
  if (lua_isnil(L, -1)) {

    // throw error, if named argument is mandatory:
    if (mandatory) return luaL_error(L, "Named argument '%s' missing", argname), 0;

    // set default value as return value, if named argument is not mandatory:
    value = default_value;

  } else {

    // throw error, if the value of the argument is not a number:
    if (lua_type(L, -1) != LUA_TNUMBER) return luaL_error(L, "Named argument '%s' is not a number", argname), 0;

    // set return value to the number:
    value = lua_tonumber(L, -1);

  }

  // remove unnecessary element from stack (not needed to avoid garbage collection):
  return value;

  // leaves no new elements on the stack
}


static int mldap_scope(
  // converts a string ("base", "onelevel", "subtree", "children") to an integer representing the LDAP scope
  // and throws an error for any unknown string

  lua_State *L,             // pointer to lua_State variable (needed to throw errors)
  const char *scope_string  // string that is either ("base", "onelevel", "subtree", "children")

  // does not affect or use the Lua stack at all
) {

  // return integer according to string value:
  if (!strcmp(scope_string, "base"))     return LDAP_SCOPE_BASE;
  if (!strcmp(scope_string, "onelevel")) return LDAP_SCOPE_ONELEVEL;
  if (!strcmp(scope_string, "subtree"))  return LDAP_SCOPE_SUBTREE;
  if (!strcmp(scope_string, "children")) return LDAP_SCOPE_CHILDREN;

  // throw error for unknown string values:
  return luaL_error(L, "Invalid LDAP scope: '%s'", scope_string), 0;

}


static int mldap_bind(lua_State *L) {
  // Lua C function that takes named arguments as a table
  // and returns a userdata object, representing the LDAP connection
  // or returns nil, an error string, and an error code (integer) on error

  // named arguments:
  // "uri"      (string)  server URI to connect to
  // "who"      (string)  DN to bind as
  // "password" (string)  password for DN to bind as
  // "timeout"  (number)  timeout in seconds

  static const int ldap_version = LDAP_VERSION3;  // providing a pointer (&ldap_version) to set LDAP protocol version 3
  const char *uri;           // C string for "uri" argument
  const char *who;           // C string for "who" argument
  struct berval cred;        // credentials ("password") are stored as struct berval
  lua_Number timeout_float;  // float (lua_Number) for timeout
  struct timeval timeout;    // timeout is stored as struct timeval
  int ldap_error;            // LDAP error code (as returned by libldap calls)
  LDAP *ldp;                 // pointer to an opaque OpenLDAP structure representing the connection
  LDAP **ldp_ptr;            // pointer to a Lua userdata structure (that only contains the 'ldp' pointer)

  // throw error if first argument is not a table:
  if (lua_type(L, 1) != LUA_TTABLE) {
    luaL_error(L, "Argument to function 'bind' is not a table.");
  }

  // extract arguments:
  uri = mldap_get_named_string_arg(L, 1, "uri", true);
  who = mldap_get_named_string_arg(L, 1, "who", false);
  cred.bv_val = mldap_get_named_string_arg(L, 1, "password", false);
  if (cred.bv_val) cred.bv_len = strlen(cred.bv_val);
  else cred.bv_len = 0;
  timeout_float = mldap_get_named_number_arg(L, 1, "timeout", false, -1);
  timeout.tv_sec = timeout_float;
  timeout.tv_usec = (timeout_float - timeout.tv_sec) * 1000000;

  // initialize OpenLDAP structure and provide URI for connection:
  ldap_error = ldap_initialize(&ldp, uri);
  // on error, jump to label "mldap_queryconn_error1", as no ldap_unbind_ext_s() is needed:
  if (ldap_error != LDAP_SUCCESS) goto mldap_queryconn_error1;

  // set LDAP protocol version 3:
  ldap_error = ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);
  // on error, jump to label "mldap_queryconn_error2", as ldap_unbind_ext_s() must be called:
  if (ldap_error != LDAP_SUCCESS) goto mldap_queryconn_error2;

  // set timeout for asynchronous OpenLDAP library calls:
  ldap_error = ldap_set_option(ldp, LDAP_OPT_TIMEOUT, &timeout);
  // on error, jump to label "mldap_queryconn_error2", as ldap_unbind_ext_s() must be called:
  if (ldap_error != LDAP_SUCCESS) goto mldap_queryconn_error2;

  // connect to LDAP server:
  ldap_error = ldap_sasl_bind_s(
    ldp,               // pointer to opaque OpenLDAP structure representing the connection
    who,               // DN to bind as
    LDAP_SASL_SIMPLE,  // SASL mechanism "simple" for password authentication
    &cred,             // password as struct berval
    NULL,              // no server controls
    NULL,              // no client controls
    NULL               // do not store server credentials
  );

  // error handling:
  if (ldap_error != LDAP_SUCCESS) {

    // error label to jump to, if a call of ldap_unbind_ext_s() is required:
    mldap_queryconn_error2:

    // close connection and free resources:
    ldap_unbind_ext_s(ldp, NULL, NULL);

    // error label to jump to, if no call of ldap_unbind_ext_s() is required:
    mldap_queryconn_error1:

    // return three values:
    lua_pushnil(L);                                  // return nil as first value
    lua_pushstring(L, ldap_err2string(ldap_error));  // return error string as second value
    lua_pushinteger(L, ldap_error);                  // return error code (integer) as third value
    return 3;

  }

  // create new Lua userdata object (that will contain the 'ldp' pointer) on top of stack:
  ldp_ptr = lua_newuserdata(L, sizeof(LDAP *));

  // set metatable of Lua userdata object:
  luaL_setmetatable(L, MLDAP_REGKEY "connection_metatable");

  // write contents of Lua userdata object (the 'ldp' pointer):
  *ldp_ptr = ldp;

  // return Lua userdata object from top of stack:
  return 1;

}


static int mldap_search(lua_State *L) {
  // Lua C function used as "search" method of Lua userdata object representing the LDAP connection
  // that takes a Lua userdata object (the LDAP connection) as first argument,
  // a table with named arguments as second argument,
  // and returns a result table on success (see below)
  // or returns nil, an error string, and an error code (integer) on error

  // named arguments:
  // "base"  (string)   DN of the entry at which to start the search
  // "scope" (string)   scope of the search, one of:
  //                      "base" to search the object itself
  //                      "onelevel" to search the object's immediate children
  //                      "subtree" to search the object and all its descendants
  //                      "children" to search all of the descendants
  // "filter" (string)  string representation of the filter to apply in the search
  //                    (conforming to RFC 4515 as extended in RFC 4526)
  // "attrs"  (table)   list of attribute descriptions (each a string) to return from matching entries

  // structure of result table:
  // {
  //   { dn      = <distinguished name (DN)>,
  //     <attr1> = { <value1>, <value2>, ... },
  //     <attr2> = { <value1>, <value2>, ... },
  //     ...
  //   },
  //   { dn      = <distinguished name (DN)>,
  //     <attr1> = { <value1>, <value2>, ... },
  //     <attr2> = { <value1>, <value2>, ... },
  //     ...
  //   },
  //   ...
  // }

  const char *base;          // C string for "base" argument
  const char *scope_string;  // C string for "scope" argument
  int scope;                 // integer representing the scope
  const char *filter;        // C string for "filter" argument
  size_t nattrs;             // number of attributes in "attrs" argument
  char **attrs;              // C string array of "attrs" argument
  size_t attr_idx;           // index variable for building the C string array of "attrs"
  int ldap_error;            // LDAP error code (as returned by libldap calls)
  LDAP **ldp_ptr;            // pointer to a pointer to the OpenLDAP structure representing the connection
  LDAPMessage *res;          // pointer to the result of ldap_search_ext_s() call
  LDAPMessage *ent;          // pointer to an entry in the result of ldap_search_ext_s() call
  int i;                     // integer to fill the Lua table returned as result

  // truncate the Lua stack to 2 elements:
  lua_settop(L, 2);

  // check if the first argument is a Lua userdata object with the correct metatable
  // and get a C pointer to that userdata object:
  ldp_ptr = luaL_checkudata(L, 1, MLDAP_REGKEY "connection_metatable");

  // throw an error, if the connection has already been closed:
  if (!*ldp_ptr) {
    return luaL_error(L, "LDAP connection has already been closed");
  }

  // check if the second argument is a table, and throw an error if it's not a table:
  if (lua_type(L, 2) != LUA_TTABLE) {
    luaL_error(L, "Argument to function 'bind' is not a table.");
  }

  // extract named arguments (requires memory allocation for 'attrs'):
  base = mldap_get_named_string_arg(L, 2, "base", true);  // pushed to 3
  scope_string = mldap_get_named_string_arg(L, 2, "scope", true);  // pushed to 4
  scope = mldap_scope(L, scope_string);
  lua_pop(L, 1);  // removes stack element 4
  filter = mldap_get_named_string_arg(L, 2, "filter", false);  // pushed to 4
  lua_getfield(L, 2, "attrs");  // pushed to 5
  nattrs = lua_len(L, -1);
  attrs = calloc(nattrs + 1, sizeof(char *));  // memory allocation, +1 for terminating NULL
  if (!attrs) return luaL_error(L, "Memory allocation error in C function 'mldap_queryconn'");
  for (attr_idx=0; attr_idx<nattrs; attr_idx++) {
    lua_pushinteger(L, attr_idx+1);
    lua_gettable(L, 5);  // pushed onto variable stack position
    if (lua_type(L, -1) != LUA_TSTRING) {
      free(attrs);
      return luaL_error(L, "Element in attribute list is not a string");
    }
    attrs[i] = lua_tostring(L, -1);
  }
  // attrs[nattrs] = NULL;  // unnecessary due to calloc

  // execute LDAP search and store pointer to the result in 'res':
  ldap_error = ldap_search_ext_s(
    *ldp_ptr,  // pointer to the opaque OpenLDAP structure representing the connection
    base,      // DN of the entry at which to start the search
    scope,     // scope of the search
    filter,    // string representation of the filter to apply in the search
    attrs,     // array of attribute descriptions (array of strings)
    0,         // do not only request attribute descriptions but also values
    NULL,      // no server controls
    NULL,      // no client controls
    NULL,      // do not set another timeout
    0,         // no sizelimit
    &res       // store result in 'res'
  );

  // free data structures that have been allocated for the 'attrs' array:
  free(attrs);

  // return nil, an error string, and an error code (integer) in case of error:
  if (ldap_error != LDAP_SUCCESS) {
    lua_pushnil(L);
    lua_pushstring(L, ldap_err2string(ldap_error));
    lua_pushinteger(L, ldap_error);
    return 3;
  }

  // clear Lua stack:
  lua_settop(L, 0);

  // create result table for all entries at stack position 1:
  lua_newtable(L);

  // use ldap_first_entry() and ldap_next_entry() functions
  // to iterate through all entries in the result 'res'
  // and count 'i' from 1 up:
  for (
    ent=ldap_first_entry(*ldp_ptr, res), i=1;
    ent;
    ent=ldap_next_entry(*ldp_ptr, ent), i++
  ) {

    char *attr;       // name of attribute
    BerElement *ber;  // structure to iterate through attributes
    char *dn;         // LDAP entry name (distinguished name)

    // create result table for one entry at stack position 2:
    lua_newtable(L);

    // use ldap_first_attribute() and ldap_next_attribute()
    // as well as 'BerElement *ber' to iterate through all attributes
    // ('ber' must be free'd with ber_free(ber, 0) call later):
    for (
      attr=ldap_first_attribute(*ldp_ptr, ent, &ber);
      attr;
      attr=ldap_next_attribute(*ldp_ptr, ent, ber)
    ) {

      struct berval **vals;  // pointer to (first element of) array of values
      struct berval **val;   // pointer to a value represented as 'struct berval' structure
      int j;                 // integer to fill a Lua table

      // push the attribute name to Lua stack position 3:
      lua_pushstring(L, attr);

      // create a new table for the attribute's values on Lua stack position 4:
      lua_newtable(L);

      // call ldap_get_values_len() to obtain the values
      // (required to be free'd later with ldap_value_free_len()):
      vals = ldap_get_values_len(*ldp_ptr, ent, attr);

      // iterate through values and count 'j' from 1 up:
      for (val=vals, j=1; *val; val++, j++) {

        // push value to Lua stack position 5:
        lua_pushlstring(L, (*val)->bv_val, (*val)->bv_len);

        // pop value from Lua stack position 5
        // and store it in table on Lua stack position 4:
        lua_rawseti(L, 4, j);

      }

      // free data structure of values:
      ldap_value_free_len(vals);

      // pop attribute name from Lua stack position 3
      // and pop value table from Lua stack position 4
      // and store them in result table for entry at Lua stack position 2:
      lua_settable(L, 2);

    }

    // free 'BerElement *ber' stucture that has been used to iterate through all attributes
    // (second argument is zero due to manpage of ldap_first_attribute()):
    ber_free(ber, 0);

    // store distinguished name (DN) on Lua stack position 3
    // (aquired memory is free'd with ldap_memfree()):
    dn = ldap_get_dn(*ldp_ptr, ent);
    lua_pushstring(L, dn);
    ldap_memfree(dn);

    // pop distinguished name (DN) from Lua stack position 3
    // and store it in field "dn" of entry result table at stack position 2
    lua_setfield(L, 2, "dn");

    // pop entry result table from Lua stack position 2
    // and store it in table at stack position 1:
    lua_rawseti(L, 1, i);
    
  }

  // return result table from top of Lua stack (stack position 1):
  return 1;

}

static int mldap_unbind(lua_State *L) {
  // Lua C function used as "unbind" function of module and "unbind" method of Lua userdata object
  // closing the LDAP connection (if still open)
  // returning nothing

  LDAP **ldp_ptr;  // pointer to a pointer to the OpenLDAP structure representing the connection

  // check if the first argument is a Lua userdata object with the correct metatable
  // and get a C pointer to that userdata object:
  ldp_ptr = luaL_checkudata(L, 1, MLDAP_REGKEY "connection_metatable");

  // check if the Lua userdata object still contains a pointer:
  if (*ldp_ptr) {

    // if it does, then call ldap_unbind_ext_s():
    ldap_unbind_ext_s(
      *ldp_ptr,  // pointer to the opaque OpenLDAP structure representing the connection
      NULL,      // no server controls
      NULL       // no client controls
    );

    // store NULL pointer in Lua userdata to mark connection as closed
    *ldp_ptr = NULL;
  }

  // returning nothing:
  return 0;

}


// registration information for library functions:
static const struct luaL_Reg mldap_module_functions[] = {
  {"bind", mldap_bind},
  {"unbind", mldap_unbind},
  {NULL, NULL}
};


// registration information for methods of connection object: 
static const struct luaL_Reg mldap_connection_methods[] = {
  {"search", mldap_search},
  {"unbind", mldap_unbind},
  {NULL, NULL}
};


// registration information for connection metatable: 
static const struct luaL_Reg mldap_connection_metamethods[] = {
  {"__gc", mldap_unbind},
  {NULL, NULL}
};


// luaopen function to initialize/register library:
int luaopen_mldap(lua_State *L) {

  // clear Lua stack:
  lua_settop(L, 0);

  // create table with library functions on Lua stack position 1:
  luaL_newlib(L, mldap_module_functions);

  // create metatable for connection objects on Lua stack position 2:
  luaL_newlib(L, mldap_connection_metamethods);

  // create table with methods for connection object on Lua stack position 3:
  luaL_newlib(L, mldap_connection_methods);

  // pop table with methods for connection object from Lua stack position 3
  // and store it as "__index" in metatable:
  lua_setfield(L, 2, "__index");

  // pop table with metatable for connection objects from Lua stack position 2
  // and store it in the Lua registry:
  lua_setfield(L, LUA_REGISTRYINDEX, MLDAP_REGKEY "connection_metatable");

  // create table for error code mappings on Lua stack position 2:
  lua_newtable(L);

  // fill table for error code mappings
  // that maps integer error codes to error code strings
  // and vice versa:
  mldap_set_errorcodes(L);

  // pop table for error code mappings from Lua stack position 2
  // and store it as "errorcodes" in table with library functions:
  lua_setfield(L, 1, "errorcodes");

  // return table with library functions from top of Lua stack:
  return 1;

}
