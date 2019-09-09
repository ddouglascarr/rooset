
volatile sig_atomic_t moonbr_io_sigterm_flag;
void moonbr_io_pushhandle(lua_State *L, int fd);
void moonbr_io_closehandle(lua_State *L, int idx, int reset);
int moonbr_io_catch_sigterm(lua_State *L);
int luaopen_moonbridge_io(lua_State *L);

