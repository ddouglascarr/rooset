
#ifndef __has_include
#define __has_include(x) 0
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#if defined(__linux__) || __has_include(<bsd/unistd.h>)
#include <bsd/unistd.h>
#endif

#ifdef MOONBR_IO_USE_TLS
#include <tls.h>
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <assert.h>

#define MOONBR_IO_MAXSTRERRORLEN 80
#define MOONBR_IO_READBUFLEN 4096
#define MOONBR_IO_WRITEBUFLEN 4096

#define MOONBR_IO_LISTEN_BACKLOG 1024

#define MOONBR_IO_STRERROR_R_MSG "Error detail unavailable due to noncompliant strerror_r() implementation"
#define moonbr_io_prepare_errmsg() \
  char errmsg[MOONBR_IO_MAXSTRERRORLEN] = MOONBR_IO_STRERROR_R_MSG; \
  strerror_r(errno, errmsg, MOONBR_IO_MAXSTRERRORLEN)
#define moonbr_io_return_prepared_errmsg() \
  lua_pushnil(L); \
  lua_pushstring(L, errmsg); \
  return 2
#define moonbr_io_return_errmsg() \
  do { \
    moonbr_io_prepare_errmsg(); \
    moonbr_io_return_prepared_errmsg(); \
  } while (0)

#define MOONBR_IO_HANDLE_MT_REGKEY "moonbridge_io_handle"
#define MOONBR_IO_HANDLE_PUBLIC_MT_REGKEY "moonbridge_io_handle_public"
#define MOONBR_IO_LISTENER_MT_REGKEY "moonbridge_io_listener"
#define MOONBR_IO_CHILD_MT_REGKEY "moonbridge_io_child"
#define MOONBR_IO_CHILD_PT_REGKEY "moonbridge_io_child_pt"

#ifdef MOONBR_IO_USE_TLS

#define MOONBR_IO_TLSCONF_MT_REGKEY "moonbridge_io_tlsconf"

typedef struct {
  struct tls_config *config;
  int server;
} moonbr_io_tlsconf_t;

#endif /* MOONBR_IO_USE_TLS */

typedef struct {
  int fd;
  int issock;
  sa_family_t addrfam;
  int finished;
  int closed;
  int nonblocking;
  int nopush;
  int readerr;
  int readbufin;
  int readbufout;
  int writeerr;
  size_t writeleft;
  size_t flushedleft;
#if LUA_VERSION_NUM >= 503
  lua_Integer writeqin;
  lua_Integer writeqout;
#else
  int writeqin;
  int writeqout;
#endif
  size_t writeqoff;
  int writebufin;
  int writebufout;
  char readbuf[MOONBR_IO_READBUFLEN];
  char writebuf[MOONBR_IO_WRITEBUFLEN];
#ifdef MOONBR_IO_USE_TLS
  struct tls *tls;
  struct tls *servertls;
  int tlshandshake;
  int tlsclosing;
#endif
} moonbr_io_handle_t;

typedef struct {
  int fd;
  sa_family_t addrfam;
  int nonblocking;
} moonbr_io_listener_t;

typedef struct {
  pid_t pid;
} moonbr_io_child_t;

volatile sig_atomic_t moonbr_io_sigterm_flag = 0;
volatile sig_atomic_t moonbr_io_sigchld_flag = 0;

static int moonbr_io_yield(lua_State *L) {
  return lua_yield(L, lua_gettop(L));
}

#if LUA_VERSION_NUM >= 503
static int moonbr_io_cont_returnall(lua_State *L, int status, lua_KContext ctx) {
#else
static int moonbr_io_cont_returnall(lua_State *L) {
#endif
  return lua_gettop(L);
}

#define moonbr_io_yield_wrapper(yieldfunc, callfunc) \
  static int yieldfunc(lua_State *L) { \
    int args; \
    lua_pushcfunction(L, callfunc); \
    lua_insert(L, 1); \
    args = lua_gettop(L); \
    lua_pushcfunction(L, moonbr_io_yield); \
    lua_insert(L, 3); \
    lua_callk(L, args, LUA_MULTRET, 0, moonbr_io_cont_returnall); \
    return lua_gettop(L); \
  }

static int moonbr_io_handle_set_nonblocking(lua_State *L, moonbr_io_handle_t *handle, int nonblocking) {
  int flags;
  if (handle->nonblocking == nonblocking) return 0;
  flags = fcntl(handle->fd, F_GETFL, 0);
  if (flags == -1) return -1;
  if (nonblocking) flags |= O_NONBLOCK;
  else flags &= ~O_NONBLOCK;
  if (fcntl(handle->fd, F_SETFL, flags) == -1) return -1;
  handle->nonblocking = nonblocking;
  return 0;
}

static int moonbr_io_handle_set_linger(lua_State *L, moonbr_io_handle_t *handle, int timeout) {
  struct linger lingerval = { 0, };
  if (!handle->issock) return 0;
  if (timeout >= 0) {
    lingerval.l_onoff = 1;
    lingerval.l_linger = timeout;
  }
  if (setsockopt(handle->fd, SOL_SOCKET, SO_LINGER, &lingerval, sizeof(lingerval))) return -1;
  return 0;
}

static inline int moonbr_io_handle_set_nopush(lua_State *L, moonbr_io_handle_t *handle, int nopush) {
#if defined(TCP_NOPUSH) || defined(TCP_CORK)
  if (
    !(handle->addrfam == AF_INET6 || handle->addrfam == AF_INET) ||
    handle->nopush == nopush
  ) return 0;
#if defined(TCP_NOPUSH)
  if (setsockopt(handle->fd, IPPROTO_TCP, TCP_NOPUSH, &nopush, sizeof(nopush))) return -1;
#elif defined(TCP_CORK)
  if (setsockopt(handle->fd, IPPROTO_TCP, TCP_CORK, &nopush, sizeof(nopush))) return -1;
#endif
  handle->nopush = nopush;
#else
#warning Neither TCP_NOPUSH nor TCP_CORK is available
#endif
  return 0;
}

static int moonbr_io_read_impl(lua_State *L, int nonblocking, int drain) {
  moonbr_io_handle_t *handle;
  lua_Integer maxread;
  const char *terminatorstr;
  size_t terminatorlen;
  char terminator = 0;  /* initialize to avoid compiler warning */
  luaL_Buffer luabuf;
  size_t luabufcnt = 0;
  int remaining;
  char *terminatorpos;
  ssize_t bytesread;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  maxread = luaL_optinteger(L, 2, -1);
  terminatorstr = luaL_optlstring(L, 3, "", &terminatorlen);
  if (terminatorlen) {
    luaL_argcheck(L, terminatorlen == 1, 3, "single byte expected");
    terminator = terminatorstr[0];
  }
  lua_settop(L, 1);  /* return handle on drain, terminator string may be garbage collected */
  if (handle->closed) luaL_error(L, "Attempt to read from a closed I/O handle");
  if (handle->readerr) {
    lua_pushnil(L);
    lua_pushliteral(L, "Previous read error");
    return 2;
  }
  if (handle->fd < 0) {
    /* fake EOF to simulate shutdown */
    if (!drain) lua_pushliteral(L, "");
    else lua_pushinteger(L, 0);
    lua_pushliteral(L, "eof");
    return 2;
  }
  handle->readerr = 1;
  if (moonbr_io_handle_set_nonblocking(L, handle, nonblocking)) moonbr_io_return_errmsg();
  if (!drain) luaL_buffinit(L, &luabuf);
  while (1) {
    remaining = -1;
    terminatorpos = NULL;
    if (
      maxread >= 0 &&
      handle->readbufin - handle->readbufout >= (size_t)maxread - luabufcnt
    ) {
      remaining = (size_t)maxread - luabufcnt;
      if (terminatorlen) {
        terminatorpos = memchr(
          handle->readbuf + handle->readbufout,
          terminator,
          remaining
        );
      }
    } else if (terminatorlen) {
      terminatorpos = memchr(
        handle->readbuf + handle->readbufout,
        terminator,
        handle->readbufin - handle->readbufout
      );
    }
    if (terminatorpos) remaining = 1 + (
      terminatorpos - (handle->readbuf + handle->readbufout)
    );
    if (remaining >= 0) {
      if (!drain) {
        luaL_addlstring(
          &luabuf,
          handle->readbuf + handle->readbufout,
          remaining
        );
        luaL_pushresult(&luabuf);
      } else {
        lua_pushinteger(L, luabufcnt + remaining);
      }
      if (terminatorpos) lua_pushliteral(L, "term");
      else lua_pushliteral(L, "maxlen");
      handle->readbufout += remaining;
      if (handle->readbufout == handle->readbufin) {
        handle->readbufin = 0;
        handle->readbufout = 0;
      }
      handle->readerr = 0;
      return 2;
    }
    if (!drain) luaL_addlstring(
      &luabuf,
      handle->readbuf + handle->readbufout,
      handle->readbufin - handle->readbufout
    );
    luabufcnt += handle->readbufin - handle->readbufout;
    handle->readbufout = 0;
#ifdef MOONBR_IO_USE_TLS
    if (handle->tls) {
      do {
        if (!handle->tlshandshake) {
          do bytesread = tls_handshake(handle->tls);
          while (!nonblocking && (bytesread == TLS_WANT_POLLIN || bytesread == TLS_WANT_POLLOUT));
          if (bytesread == TLS_WANT_POLLIN || bytesread == TLS_WANT_POLLOUT) {
            handle->tlshandshake = bytesread;
            errno = EAGAIN;
            break;
          }
          if (bytesread < 0) {
            lua_pushnil(L);
            lua_pushstring(L, tls_error(handle->tls));
            return 2;
          }
          handle->tlshandshake = 1;
        }
        do bytesread = tls_read(handle->tls, handle->readbuf, MOONBR_IO_READBUFLEN);
        while (!nonblocking && (bytesread == TLS_WANT_POLLIN || bytesread == TLS_WANT_POLLOUT));
        if (bytesread == TLS_WANT_POLLIN || bytesread == TLS_WANT_POLLOUT) {
          errno = EAGAIN;
          break;
        }
        if (bytesread < 0) {
          lua_pushnil(L);
          lua_pushstring(L, tls_error(handle->tls));
          return 2;
        }
      } while (0);
    }
    else
#endif
    do bytesread = read(handle->fd, handle->readbuf, MOONBR_IO_READBUFLEN);
    while (bytesread < 0 && (errno == EINTR));
    if (
      bytesread == 0 || (
        nonblocking &&
        bytesread < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)
      )
    ) {
      handle->readbufin = 0;
      if (!drain) luaL_pushresult(&luabuf);
      else lua_pushinteger(L, luabufcnt);
      if (bytesread == 0) lua_pushliteral(L, "eof");
      else lua_pushliteral(L, "block");
      handle->readerr = 0;
      return 2;
    }
    if (bytesread < 0) moonbr_io_return_errmsg();
    handle->readbufin = bytesread;
  }
}

static int moonbr_io_read(lua_State *L) {
  return moonbr_io_read_impl(L, 0, 0);
}

static int moonbr_io_read_nb(lua_State *L) {
  return moonbr_io_read_impl(L, 1, 0);
}

static int moonbr_io_drain(lua_State *L) {
  return moonbr_io_read_impl(L, 0, 1);
}

static int moonbr_io_drain_nb(lua_State *L) {
  return moonbr_io_read_impl(L, 1, 1);
}

#if LUA_VERSION_NUM >= 503
static int moonbr_io_read_cont(lua_State *L, int status, lua_KContext ctx) {
#else
static int moonbr_io_read_cont(lua_State *L) {
#endif
  lua_Integer remaining;
  size_t len;
#if !(LUA_VERSION_NUM >= 503)
  int ctx = 0;
  lua_getctx(L, &ctx);
#endif
  remaining = lua_tointeger(L, 3);
  while (1) {
    lua_pushcfunction(L, moonbr_io_read_nb);
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 3);
    lua_pushvalue(L, 4);
    lua_call(L, 3, 2);
    if (lua_isnil(L, -2)) return 2;
    lua_insert(L, -2);
    len = lua_rawlen(L, -1);
    if (ctx == 0) {
      lua_replace(L, 5);
      ctx = 1;
    } else if (ctx == 1) {
      lua_pushvalue(L, 5);
      lua_newtable(L);
      lua_replace(L, 5);
      lua_rawseti(L, 5, 2);
      lua_rawseti(L, 5, 1);
      ctx = 2;
    } else {
      lua_rawseti(L, 5, lua_rawlen(L, 5) + 1);
    }
    if (strcmp(lua_tostring(L, -1), "block") != 0) break;
    lua_pop(L, 1);
    if (remaining >= 0 && len) {
      remaining -= len;
      lua_pushinteger(L, remaining);
      lua_replace(L, 3);
    }
    lua_pushvalue(L, 2);
    lua_callk(L, 0, 0, ctx, moonbr_io_read_cont);
  }
  if (ctx == 1) {
    lua_pushvalue(L, 5);
  } else {
    luaL_Buffer buf;
    lua_Integer i, chunkcount;
    chunkcount = lua_rawlen(L, 5);
    luaL_buffinit(L, &buf);
    for (i=1; i<=chunkcount && i>0; i++) {
      lua_rawgeti(L, 5, i);
      luaL_addvalue(&buf);
    }
    luaL_pushresult(&buf);
  }
  lua_pushvalue(L, -2);
  return 2;
}

static int moonbr_io_read_call(lua_State *L) {
  lua_settop(L, 4);
  lua_pushnil(L);
#if LUA_VERSION_NUM >= 503
  return moonbr_io_read_cont(L, 0, 0);
#else
  return moonbr_io_read_cont(L);
#endif
}

moonbr_io_yield_wrapper(moonbr_io_read_yield, moonbr_io_read_call);

#if LUA_VERSION_NUM >= 503
static int moonbr_io_drain_cont(lua_State *L, int status, lua_KContext ctx) {
#else
static int moonbr_io_drain_cont(lua_State *L) {
#endif
  lua_Integer remaining, len;
  size_t totallen = 0;
#if !(LUA_VERSION_NUM >= 503)
  int ctx = 0;
  lua_getctx(L, &ctx);
#endif
  remaining = lua_tointeger(L, 3);
  while (1) {
    lua_pushcfunction(L, moonbr_io_drain_nb);
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 3);
    lua_pushvalue(L, 4);
    lua_call(L, 3, 2);
    if (lua_isnil(L, -2)) return 2;
    lua_insert(L, -2);
    len = lua_tointeger(L, -1);
    lua_pop(L, 1);
    totallen += len;
    if (strcmp(lua_tostring(L, -1), "block") != 0) break;
    lua_pop(L, 1);
    if (remaining >= 0 && len) {
      remaining -= len;
      lua_pushinteger(L, remaining);
      lua_replace(L, 3);
    }
    lua_pushvalue(L, 2);
    lua_callk(L, 0, 0, ctx, moonbr_io_drain_cont);
  }
  lua_pushinteger(L, totallen);
  lua_pushvalue(L, -2);
  return 2;
}

static int moonbr_io_drain_call(lua_State *L) {
#if LUA_VERSION_NUM >= 503
  return moonbr_io_drain_cont(L, 0, 0);
#else
  return moonbr_io_drain_cont(L);
#endif
}

moonbr_io_yield_wrapper(moonbr_io_drain_yield, moonbr_io_drain_call);

#ifdef MOONBR_IO_USE_TLS

#define moonbr_io_write_tls(buf, buflen) \
  if (handle->tls) { \
    do { \
      if (!handle->tlshandshake) { \
        do written = tls_handshake(handle->tls); \
        while (!nonblocking && (written == TLS_WANT_POLLIN || written == TLS_WANT_POLLOUT)); \
        if (written == TLS_WANT_POLLIN || written == TLS_WANT_POLLOUT) { \
          handle->tlshandshake = written; \
          errno = EAGAIN; \
          break; \
        } \
        if (written < 0) { \
          lua_pushnil(L); \
          lua_pushstring(L, tls_error(handle->tls)); \
          return 2; \
        } \
        handle->tlshandshake = 1; \
      } \
      do written = tls_write(handle->tls, (buf), (buflen)); \
      while (!nonblocking && (written == TLS_WANT_POLLIN || written == TLS_WANT_POLLOUT)); \
      if (written == TLS_WANT_POLLIN || written == TLS_WANT_POLLOUT) { \
        errno = EAGAIN; \
        break; \
      } \
      if (written < 0) { \
        lua_pushnil(L); \
        lua_pushstring(L, tls_error(handle->tls)); \
        return 2; \
      } \
    } while (0); \
  } \
  else

#endif /* MOONBR_IO_USE_TLS */

static int moonbr_io_write_impl(lua_State *L, int nonblocking, int flush) {
  moonbr_io_handle_t *handle;
  int i, top;
  const char *str;
  size_t strlen;
  ssize_t written;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  if (handle->closed) luaL_error(L, "Attempt to write to a closed I/O handle");
  if (handle->finished) luaL_error(L, "Attempt to write to a finished I/O handle");
  if (handle->writeerr) {
    lua_pushnil(L);
    lua_pushliteral(L, "Previous write error");
    return 2;
  }
  handle->writeerr = 1;
  if (moonbr_io_handle_set_nonblocking(L, handle, nonblocking)) moonbr_io_return_errmsg();
  top = lua_gettop(L);
  lua_getuservalue(L, 1);
  lua_getfield(L, -1, "writequeue");
  for (i=2; i<=top; i++) {
    luaL_checklstring(L, i, &strlen);
    lua_pushvalue(L, i);
    lua_rawseti(L, -2, handle->writeqin++);
    handle->writeleft += strlen;
  }
  if (flush) handle->flushedleft = handle->writeleft;
  while (handle->writeqout != handle->writeqin) {
    lua_rawgeti(L, -1, handle->writeqout);
    str = lua_tolstring(L, -1, &strlen);
    while (handle->writeqoff < strlen) {
      if (
        strlen - handle->writeqoff <
        MOONBR_IO_WRITEBUFLEN - handle->writebufin
      ) {
        memcpy(
          handle->writebuf + handle->writebufin,
          str + handle->writeqoff,
          strlen - handle->writeqoff
        );
        handle->writebufin += strlen - handle->writeqoff;
        break;
      } else {
        memcpy(
          handle->writebuf + handle->writebufin,
          str + handle->writeqoff,
          MOONBR_IO_WRITEBUFLEN - handle->writebufin
        );
        handle->writeqoff += MOONBR_IO_WRITEBUFLEN - handle->writebufin;
        handle->writebufin = MOONBR_IO_WRITEBUFLEN;
        while (handle->writebufout < MOONBR_IO_WRITEBUFLEN) {
          if (moonbr_io_handle_set_nopush(L, handle, 1)) moonbr_io_return_errmsg();
#ifdef MOONBR_IO_USE_TLS
          moonbr_io_write_tls(
            handle->writebuf + handle->writebufout,
            MOONBR_IO_WRITEBUFLEN - handle->writebufout
          )
#endif
          written = write(
            handle->fd,
            handle->writebuf + handle->writebufout,
            MOONBR_IO_WRITEBUFLEN - handle->writebufout
          );
          if (written < 0) {
            if (nonblocking && (errno == EAGAIN || errno == EWOULDBLOCK)) {
              goto moonbr_io_write_impl_block;
            } else if (errno != EINTR) moonbr_io_return_errmsg();
          } else {
            handle->writebufout += written;
            handle->writeleft -= written;
            if (handle->flushedleft) {
              if (written >= handle->flushedleft) {
                handle->flushedleft = 0;
                if (moonbr_io_handle_set_nopush(L, handle, 0)) moonbr_io_return_errmsg();
              } else {
                handle->flushedleft -= written;
              }
            }
          }
        }
        handle->writebufin = 0;
        handle->writebufout = 0;
      }
    }
    handle->writeqoff = 0;
    lua_pop(L, 1);
    lua_pushnil(L);
    lua_rawseti(L, -2, handle->writeqout++);
  }
  while (handle->flushedleft) {
    if (moonbr_io_handle_set_nopush(L, handle, 1)) moonbr_io_return_errmsg();
#ifdef MOONBR_IO_USE_TLS
    moonbr_io_write_tls(
      handle->writebuf + handle->writebufout,
      handle->writebufin - handle->writebufout
    )
#endif
    written = write(
      handle->fd,
      handle->writebuf + handle->writebufout,
      handle->writebufin - handle->writebufout
    );
    if (written < 0) {
      if (nonblocking && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        goto moonbr_io_write_impl_block;
      } else if (errno != EINTR) moonbr_io_return_errmsg();
    } else {
      handle->writebufout += written;
      handle->writeleft -= written;
      if (handle->flushedleft) {
        if (written >= handle->flushedleft) {
          handle->flushedleft = 0;
          if (moonbr_io_handle_set_nopush(L, handle, 0)) moonbr_io_return_errmsg();
        } else {
          handle->flushedleft -= written;
        }
      }
    }
  }
  if (handle->writebufout == handle->writebufin) {
    handle->writebufin = 0;
    handle->writebufout = 0;
  }
  if (nonblocking) lua_pushinteger(L, 0);
  else lua_pushvalue(L, 1);
  handle->writeerr = 0;
  return 1;
  moonbr_io_write_impl_block:
  lua_pushinteger(L, handle->writeleft);
  handle->writeerr = 0;
  return 1;
}

static int moonbr_io_write(lua_State *L) {
  return moonbr_io_write_impl(L, 0, 0);
}

static int moonbr_io_write_nb(lua_State *L) {
  return moonbr_io_write_impl(L, 1, 0);
}

static int moonbr_io_flush(lua_State *L) {
  return moonbr_io_write_impl(L, 0, 1);
}

static int moonbr_io_flush_nb(lua_State *L) {
  return moonbr_io_write_impl(L, 1, 1);
}

#if LUA_VERSION_NUM >= 503
static int moonbr_io_write_cont(lua_State *L, int status, lua_KContext ctx) {
#else
static int moonbr_io_write_cont(lua_State *L) {
#endif
  while (1) {
    lua_pushcfunction(L, moonbr_io_write_nb);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 2);
    if (lua_isnil(L, -2)) return 2;
    if (!lua_tointeger(L, -2)) {
      lua_pushvalue(L, 1);
      return 1;
    }
    lua_pop(L, 2);
    lua_pushvalue(L, 2);
    lua_callk(L, 0, 0, 0, moonbr_io_write_cont);
  }
}

static int moonbr_io_write_call(lua_State *L) {
  lua_pushcfunction(L, moonbr_io_write_nb);
  lua_insert(L, 3);
  lua_pushvalue(L, 1);
  lua_insert(L, 4);
  lua_call(L, lua_gettop(L) - 3, 2);
  if (lua_isnil(L, -2)) return 2;
  if (!lua_tointeger(L, -2)) {
    lua_pushvalue(L, 1);
    return 1;
  }
#if LUA_VERSION_NUM >= 503
  return moonbr_io_write_cont(L, 0, 0);
#else
  return moonbr_io_write_cont(L);
#endif
}

moonbr_io_yield_wrapper(moonbr_io_write_yield, moonbr_io_write_call);

static int moonbr_io_flush_call(lua_State *L) {
  lua_pushcfunction(L, moonbr_io_flush_nb);
  lua_insert(L, 3);
  lua_pushvalue(L, 1);
  lua_insert(L, 4);
  lua_call(L, lua_gettop(L) - 3, 2);
  if (lua_isnil(L, -2)) return 2;
  if (!lua_tointeger(L, -2)) {
    lua_pushvalue(L, 1);
    return 1;
  }
#if LUA_VERSION_NUM >= 503
  return moonbr_io_write_cont(L, 0, 0);
#else
  return moonbr_io_write_cont(L);
#endif
}

moonbr_io_yield_wrapper(moonbr_io_flush_yield, moonbr_io_flush_call);

static int moonbr_io_finish(lua_State *L) {
  moonbr_io_handle_t *handle;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  if (handle->closed) luaL_error(L, "Attempt to finish a closed I/O handle");
  if (handle->finished) luaL_error(L, "Attempt to finish a finished I/O handle");
  if (handle->writeleft) {
    lua_pushcfunction(L, moonbr_io_flush);
    lua_pushvalue(L, 1);
    if (lua_pcall(L, 1, 2, 0)) {
      handle->finished = 1;
      lua_error(L);
    }
    if (!lua_toboolean(L, -2)) {
      handle->finished = 1;
      return 2;
    }
  }
  handle->finished = 1;
#ifdef MOONBR_IO_USE_TLS
  if ((handle->addrfam == AF_INET6 || handle->addrfam == AF_INET) && !handle->tls) {
#else
  if (handle->addrfam == AF_INET6 || handle->addrfam == AF_INET) {
#endif
    if (shutdown(handle->fd, SHUT_WR)) moonbr_io_return_errmsg();
  } else {
#ifdef MOONBR_IO_USE_TLS
    if (handle->tls) {
      int status;
      if (moonbr_io_handle_set_nonblocking(L, handle, 1)) moonbr_io_return_errmsg();
      do status = tls_close(handle->tls);
      while (status == TLS_WANT_POLLIN || status == TLS_WANT_POLLOUT);
      if (status) {
        close(handle->fd);
        handle->fd = -1;
        lua_pushnil(L);
        lua_pushstring(L, tls_error(handle->tls));
        return 2;
      }
    }
#endif
    if (close(handle->fd)) {
      handle->fd = -1;
      moonbr_io_return_errmsg();
    }
    handle->fd = -1;  /* fake EOF on read */
  }
  lua_pushboolean(L, 1);
  return 1;
}

static int moonbr_io_close_impl(lua_State *L, int nonblocking, int reset) {
  moonbr_io_handle_t *handle;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  if (handle->closed) luaL_error(L, "Attempt to close a closed I/O handle");
  if (!reset && handle->fd >= 0) {
    if (handle->writeleft) {
      lua_pushcfunction(L, nonblocking ? moonbr_io_flush_nb : moonbr_io_flush);
      lua_pushvalue(L, 1);
      if (lua_pcall(L, 1, 2, 0)) {
        handle->closed = 1;
        close(handle->fd);
        handle->fd = -1;
        lua_error(L);
      }
      if (!nonblocking) handle->closed = 1;  /* TODO: handle nonblocking case */
      if (!lua_toboolean(L, -2)) {
        close(handle->fd);
        handle->fd = -1;
        return 2;
      }
#if LUA_VERSION_NUM >= 503
      if (nonblocking && lua_tointeger(L, -2)) {
#else
      if (nonblocking && lua_tonumber(L, -2)) {
#endif
        lua_pushliteral(L, "flush");
        lua_pushvalue(L, -3);
        return 2;
      }
    } else {
      handle->closed = 1;
    }
#ifdef MOONBR_IO_USE_TLS
    if (handle->tls) {
      int status;
      if (moonbr_io_handle_set_nonblocking(L, handle, nonblocking)) moonbr_io_return_errmsg();
      do status = tls_close(handle->tls);
      while (!nonblocking && (status == TLS_WANT_POLLIN || status == TLS_WANT_POLLOUT));
      if (status == TLS_WANT_POLLIN || status == TLS_WANT_POLLOUT) {
        handle->tlsclosing = status;  /* TODO: handle polling */
        lua_pushliteral(L, "close");
        return 1;
      }
      if (status) {
        close(handle->fd);
        handle->fd = -1;
        lua_pushnil(L);
        lua_pushstring(L, tls_error(handle->tls));
        return 2;
      }
    }
#endif
    if (moonbr_io_handle_set_linger(L, handle, -1)) {
      moonbr_io_prepare_errmsg();
      close(handle->fd);
      handle->fd = -1;
      moonbr_io_return_prepared_errmsg();
    }
  } else {
    handle->closed = 1;
  }
  if (handle->fd >= 0) {
    if (close(handle->fd)) {
      handle->fd = -1;
      moonbr_io_return_errmsg();
    }
    handle->fd = -1;
  }
  lua_pushboolean(L, 1);
  return 1;

}

static int moonbr_io_close(lua_State *L) {
  return moonbr_io_close_impl(L, 0, 0);
}

static int moonbr_io_reset(lua_State *L) {
  return moonbr_io_close_impl(L, 0, 1);
}

static int moonbr_io_handlegc(lua_State *L) {
  moonbr_io_handle_t *handle;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  if (handle->fd >= 0) {
    lua_pushcfunction(L, moonbr_io_reset);
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 0);
    lua_call(L, 2, 0);
  }
#ifdef MOONBR_IO_USE_TLS
  if (handle->tls) {
    tls_free(handle->tls);
    handle->tls = NULL;
  }
  if (handle->servertls) {
    tls_free(handle->servertls);
    handle->servertls = NULL;
  }
#endif
  return 0;
}

void moonbr_io_closehandle(lua_State *L, int idx, int reset) {
  moonbr_io_handle_t *handle;
  handle = luaL_checkudata(L, idx, MOONBR_IO_HANDLE_MT_REGKEY);
  if (!handle->closed) {
    lua_pushcfunction(L, reset ? moonbr_io_reset : moonbr_io_close);
    lua_pushvalue(L, idx < 0 ? idx-1 : idx);
    lua_call(L, 1, 0);
  }
}

static int moonbr_io_pushhandle_impl(lua_State *L) {
  int *fd;
  moonbr_io_handle_t *handle;
  struct sockaddr addr;
  socklen_t addrlen;
  fd = lua_touserdata(L, 1);
  handle = lua_newuserdata(L, sizeof(moonbr_io_handle_t));
  handle->fd = -1;  /* avoid closing incomplete handle */
  addrlen = sizeof(addr);
  if (getsockname(*fd, &addr, &addrlen)) {
    if (errno != ENOTSOCK) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Unexpected error when examining socket: %s", errmsg);
    }
    handle->issock = 0;
  } else {
    handle->issock = 1;
    handle->addrfam = addr.sa_family;
  }
  handle->finished = 0;
  handle->closed = 0;
  handle->nonblocking = -1;
  handle->nopush = -1;
  handle->readerr = 0;
  handle->readbufin = 0;
  handle->readbufout = 0;
  handle->writeerr = 0;
  handle->writeleft = 0;
  handle->flushedleft = 0;
  handle->writeqin = 0;
  handle->writeqout = 0;
  handle->writeqoff = 0;
  handle->writebufin = 0;
  handle->writebufout = 0;
#ifdef MOONBR_IO_USE_TLS
  handle->tls = NULL;
  handle->servertls = NULL;
  handle->tlshandshake = 0;
  handle->tlsclosing = 0;
#endif
  handle->fd = *fd;  /* required for set_linger call */
  if (moonbr_io_handle_set_linger(L, handle, 0)) {
    moonbr_io_prepare_errmsg();
    handle->fd = -1;
    luaL_error(L, "Unexpected error while setting SO_LINGER with setsockopt: %s", errmsg);
  }
  handle->fd = -1;  /* avoid closing incomplete handle */
  luaL_setmetatable(L, MOONBR_IO_HANDLE_MT_REGKEY);
  lua_newtable(L);  // uservalue
  lua_newtable(L);
  lua_setfield(L, -2, "writequeue");
  lua_newtable(L);  // public
  if (handle->addrfam == AF_INET6) {
    struct sockaddr_in6 addr_in6;
    char addrstrbuf[INET6_ADDRSTRLEN];
    const char *addrstr;
    addrlen = sizeof(addr_in6);
    if (getsockname(*fd, (struct sockaddr *)&addr_in6, &addrlen)) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not determine local IP address/port: %s", errmsg);
    }
    if (addrlen > sizeof(addr_in6)) {
      luaL_error(L, "Could not determine local IP address/port: buffer size exceeded");
    }
    addrstr = inet_ntop(AF_INET6, addr_in6.sin6_addr.s6_addr, addrstrbuf, sizeof(addrstrbuf));
    if (!addrstr) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not format local IP address: %s", errmsg);
    } else {
      lua_pushstring(L, addrstr);
      lua_setfield(L, -2, "local_ip6");
    }
    lua_pushinteger(L, ntohs(addr_in6.sin6_port));
    lua_setfield(L, -2, "local_tcpport");
    if (getpeername(*fd, (struct sockaddr *)&addr_in6, &addrlen)) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not determine remote IP address/port: %s", errmsg);
    }
    if (addrlen > sizeof(addr_in6)) {
      luaL_error(L, "Could not determine remote IP address/port: buffer size exceeded");
    }
    addrstr = inet_ntop(AF_INET6, addr_in6.sin6_addr.s6_addr, addrstrbuf, sizeof(addrstrbuf));
    if (!addrstr) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not format remote IP address: %s", errmsg);
    } else {
      lua_pushstring(L, addrstr);
      lua_setfield(L, -2, "remote_ip6");
    }
    lua_pushinteger(L, ntohs(addr_in6.sin6_port));
    lua_setfield(L, -2, "remote_tcpport");
  } else if (handle->addrfam == AF_INET) {
    struct sockaddr_in addr_in;
    char addrstrbuf[INET_ADDRSTRLEN];
    const char *addrstr;
    addrlen = sizeof(addr_in);
    if (getsockname(*fd, (struct sockaddr *)&addr_in, &addrlen)) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not determine local IP address/port: %s", errmsg);
    }
    if (addrlen > sizeof(addr_in)) {
      luaL_error(L, "Could not determine local IP address/port: buffer size exceeded");
    }
    addrstr = inet_ntop(AF_INET, &addr_in.sin_addr.s_addr, addrstrbuf, sizeof(addrstrbuf));
    if (!addrstr) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not format local IP address: %s", errmsg);
    } else {
      lua_pushstring(L, addrstr);
      lua_setfield(L, -2, "local_ip4");
    }
    lua_pushinteger(L, ntohs(addr_in.sin_port));
    lua_setfield(L, -2, "local_tcpport");
    if (getpeername(*fd, (struct sockaddr *)&addr_in, &addrlen)) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not determine remote IP address/port: %s", errmsg);
    }
    if (addrlen > sizeof(addr_in)) {
      luaL_error(L, "Could not determine remote IP address/port: buffer size exceeded");
    }
    addrstr = inet_ntop(AF_INET, &addr_in.sin_addr.s_addr, addrstrbuf, sizeof(addrstrbuf));
    if (!addrstr) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Could not format remote IP address: %s", errmsg);
    } else {
      lua_pushstring(L, addrstr);
      lua_setfield(L, -2, "remote_ip4");
    }
    lua_pushinteger(L, ntohs(addr_in.sin_port));
    lua_setfield(L, -2, "remote_tcpport");
  }
  luaL_setmetatable(L, MOONBR_IO_HANDLE_PUBLIC_MT_REGKEY);
  lua_setfield(L, -2, "public");
  lua_setuservalue(L, -2);
  handle->fd = *fd;
  *fd = -1;  /* closing is now handled by garbage collection */
  return 1;
}

void moonbr_io_pushhandle(lua_State *L, int fd) {
  lua_pushcfunction(L, moonbr_io_pushhandle_impl);
  lua_pushlightuserdata(L, &fd);
  if (lua_pcall(L, 1, 1, 0)) {
    if (fd != -1) close(fd);  // TODO: correct to close file descriptor here?
    lua_error(L);
  }
}

static int moonbr_io_handleindex(lua_State *L) {
  luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  luaL_checkany(L, 2);
  lua_getuservalue(L, 1);
  lua_getfield(L, -1, "public");
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  return 1;
}

static int moonbr_io_handlenewindex(lua_State *L) {
  luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  luaL_checkany(L, 2);
  luaL_checkany(L, 3);
  lua_getuservalue(L, 1);
  lua_getfield(L, -1, "public");
  lua_pushvalue(L, 2);
  lua_pushvalue(L, 3);
  lua_settable(L, -3);
  return 0;
}

static int moonbr_io_localconnect_impl(lua_State *L, int nonblocking) {
  const char *path;
  struct sockaddr_un sockaddr = { .sun_family = AF_LOCAL };
  const int path_maxlen = sizeof(struct sockaddr_un) - (
    (void *)sockaddr.sun_path - (void *)&sockaddr
  ) - 1;  /* one byte for termination */
  int sock;
  path = luaL_checkstring(L, 1);
  if (strlen(path) > path_maxlen) luaL_error(L, "Path too long; only %i characters allowed", path_maxlen);
  strcpy(sockaddr.sun_path, path);
  sock = socket(
    PF_LOCAL,
    SOCK_STREAM | SOCK_CLOEXEC | (nonblocking ? SOCK_NONBLOCK : 0),
    0
  );
  if (sock < 0) moonbr_io_return_errmsg();
  if (connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
    if (!nonblocking && errno == EINTR) {
      moonbr_io_prepare_errmsg();
      close(sock);
      moonbr_io_return_prepared_errmsg();
    } else if (!(nonblocking && (errno == EINPROGRESS || errno == EINTR))) moonbr_io_return_errmsg();
  }
  moonbr_io_pushhandle(L, sock);
  return 1;
}

static int moonbr_io_localconnect(lua_State *L) {
  return moonbr_io_localconnect_impl(L, 0);
}

static int moonbr_io_localconnect_nb(lua_State *L) {
  return moonbr_io_localconnect_impl(L, 1);
}

static int moonbr_io_tcpconnect_impl(lua_State *L, int nonblocking) {
  const char *host, *port;
  struct addrinfo hints = { 0, };
  struct addrinfo *res, *addrinfo;
  int errcode;
  int sock;
  host = luaL_checkstring(L, 1);
  port = luaL_checkstring(L, 2);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_ADDRCONFIG;
  errcode = getaddrinfo(host, port, &hints, &res);
  if (errcode) {
    freeaddrinfo(res);
    if (errcode == EAI_SYSTEM) {
      moonbr_io_prepare_errmsg();
      lua_pushnil(L);
      lua_pushfstring(L, "%s: %s", gai_strerror(errcode), errmsg);
    } else {
      lua_pushnil(L);
      lua_pushstring(L, gai_strerror(errcode));
    }
    return 2;
  }
  for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
    if (addrinfo->ai_family == AF_INET6) goto moonbr_io_tcpconnect_found;
  }
  for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
    if (addrinfo->ai_family == AF_INET) goto moonbr_io_tcpconnect_found;
  }
  addrinfo = res;
  moonbr_io_tcpconnect_found:
  sock = socket(
    addrinfo->ai_family,  /* NOTE: not correctly using PF_* but AF_* constants here */
    addrinfo->ai_socktype | SOCK_CLOEXEC | (nonblocking ? SOCK_NONBLOCK : 0),
    addrinfo->ai_protocol
  );
  if (sock < 0) {
    moonbr_io_prepare_errmsg();
    freeaddrinfo(res);
    moonbr_io_return_prepared_errmsg();
  }
  if (connect(sock, addrinfo->ai_addr, addrinfo->ai_addrlen)) {
    freeaddrinfo(res);
    if (!nonblocking && errno == EINTR) {
      moonbr_io_prepare_errmsg();
      close(sock);
      moonbr_io_return_prepared_errmsg();
    } else if (!(nonblocking && (errno == EINPROGRESS || errno == EINTR))) moonbr_io_return_errmsg();
  } else {
    freeaddrinfo(res);
  }
  moonbr_io_pushhandle(L, sock);
  return 1;
}

static int moonbr_io_tcpconnect(lua_State *L) {
  return moonbr_io_tcpconnect_impl(L, 0);
}

static int moonbr_io_tcpconnect_nb(lua_State *L) {
  return moonbr_io_tcpconnect_impl(L, 1);
}

static int moonbr_io_locallisten(lua_State *L) {
  moonbr_io_listener_t *listener;
  const char *path;
  struct stat sb;
  struct sockaddr_un sockaddr = { .sun_family = AF_LOCAL };
  const int path_maxlen = sizeof(struct sockaddr_un) - (
    (void *)sockaddr.sun_path - (void *)&sockaddr
  ) - 1;  /* one byte for termination */
  int sock;
  path = luaL_checkstring(L, 1);
  if (strlen(path) > path_maxlen) luaL_error(L, "Path too long; only %i characters allowed", path_maxlen);
  strcpy(sockaddr.sun_path, path);
  if (stat(path, &sb) == 0) {
    if (S_ISSOCK(sb.st_mode)) unlink(path);
  }
  listener = lua_newuserdata(L, sizeof(moonbr_io_listener_t));
  listener->fd = -1;
  luaL_setmetatable(L, MOONBR_IO_LISTENER_MT_REGKEY);
  sock = socket(
    PF_LOCAL,
    SOCK_STREAM | SOCK_CLOEXEC,
    0
  );
  if (sock < 0) moonbr_io_return_errmsg();
  if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
    moonbr_io_prepare_errmsg();
    close(sock);
    moonbr_io_return_prepared_errmsg();
  }
  if (listen(sock, MOONBR_IO_LISTEN_BACKLOG)) {
    moonbr_io_prepare_errmsg();
    close(sock);
    moonbr_io_return_prepared_errmsg();
  }
  listener->fd = sock;
  listener->addrfam = AF_LOCAL;
  listener->nonblocking = -1;
  return 1;
}

static int moonbr_io_tcplisten(lua_State *L) {
  moonbr_io_listener_t *listener;
  const char *host, *port;
  struct addrinfo hints = { 0, };
  struct addrinfo *res, *addrinfo;
  int errcode;
  int sock;
  host = luaL_optstring(L, 1, NULL);
  port = luaL_checkstring(L, 2);
  listener = lua_newuserdata(L, sizeof(moonbr_io_listener_t));
  listener->fd = -1;
  luaL_setmetatable(L, MOONBR_IO_LISTENER_MT_REGKEY);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
  errcode = getaddrinfo(host, port, &hints, &res);
  if (errcode) {
    freeaddrinfo(res);
    if (errcode == EAI_SYSTEM) {
      moonbr_io_prepare_errmsg();
      lua_pushnil(L);
      lua_pushfstring(L, "%s: %s", gai_strerror(errcode), errmsg);
    } else {
      lua_pushnil(L);
      lua_pushstring(L, gai_strerror(errcode));
    }
    return 2;
  }
  for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
    if (addrinfo->ai_family == AF_INET6) goto moonbr_io_tcpconnect_found;
  }
  for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
    if (addrinfo->ai_family == AF_INET) goto moonbr_io_tcpconnect_found;
  }
  addrinfo = res;
  moonbr_io_tcpconnect_found:
  listener->addrfam = addrinfo->ai_family;
  sock = socket(
    addrinfo->ai_family,  /* NOTE: not correctly using PF_* but AF_* constants here */
    addrinfo->ai_socktype | SOCK_CLOEXEC,
    addrinfo->ai_protocol
  );
  if (sock < 0) {
    moonbr_io_prepare_errmsg();
    freeaddrinfo(res);
    moonbr_io_return_prepared_errmsg();
  }
  {
    static const int reuseval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseval, sizeof(reuseval))) {
      moonbr_io_prepare_errmsg();
      freeaddrinfo(res);
      close(sock);
      lua_pushnil(L);
      lua_pushfstring(L, "Error while setting SO_REUSEADDR with setsockopt: %s", errmsg);
      return 2;
    }
  }
  if (bind(sock, addrinfo->ai_addr, addrinfo->ai_addrlen)) {
    moonbr_io_prepare_errmsg();
    freeaddrinfo(res);
    close(sock);
    moonbr_io_return_prepared_errmsg();
  }
  freeaddrinfo(res);
  if (listen(sock, MOONBR_IO_LISTEN_BACKLOG)) {
    moonbr_io_prepare_errmsg();
    close(sock);
    moonbr_io_return_prepared_errmsg();
  }
  listener->fd = sock;
  listener->nonblocking = -1;
  return 1;
}

static int moonbr_io_accept_impl(lua_State *L, int nonblocking) {
  moonbr_io_listener_t *listener;
  int fd;
  listener = luaL_checkudata(L, 1, MOONBR_IO_LISTENER_MT_REGKEY);
  if (listener->fd < 0) luaL_error(L, "Attempt to use a closed listener");
  if (listener->nonblocking != nonblocking) {
    int flags;
    flags = fcntl(listener->fd, F_GETFL, 0);
    if (flags == -1) {
      moonbr_io_prepare_errmsg();
      close(listener->fd);
      listener->fd = -1;
      luaL_error(L, "Unexpected error in fcntl call: %s", errmsg);
    }
    if (nonblocking) flags |= O_NONBLOCK;
    else flags &= ~O_NONBLOCK;
    if (fcntl(listener->fd, F_SETFL, flags) == -1) {
      moonbr_io_prepare_errmsg();
      close(listener->fd);
      listener->fd = -1;
      luaL_error(L, "Unexpected error in fcntl call: %s", errmsg);
    }
    listener->nonblocking = nonblocking;
  }
  while (1) {
#if defined(__linux__) && !defined(_GNU_SOURCE)
    fd = accept(listener->fd, NULL, NULL);
    if (fd != -1) {
      if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
        moonbr_io_prepare_errmsg();
        close(listener->fd);
        listener->fd = -1;
        close(fd);
        luaL_error(L, "Unexpected error in fcntl call: %s", errmsg);
      }
    }
#else
    fd = accept4(listener->fd, NULL, NULL, SOCK_CLOEXEC);
#endif
    if (fd < 0) {
      if (nonblocking && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        lua_pushboolean(L, 0);
        lua_pushliteral(L, "No incoming connection pending");
        return 2;
      } else if (errno != EINTR) moonbr_io_return_errmsg();
    } else {
      moonbr_io_pushhandle(L, fd);
      return 1;
    }
  }
}

static int moonbr_io_accept(lua_State *L) {
  return moonbr_io_accept_impl(L, 0);
}

static int moonbr_io_accept_nb(lua_State *L) {
  return moonbr_io_accept_impl(L, 1);
}

static int moonbr_io_unlisten(lua_State *L) {
  moonbr_io_listener_t *listener;
  struct sockaddr_un addr;
  socklen_t addrlen;
  struct stat sb;
  listener = luaL_checkudata(L, 1, MOONBR_IO_LISTENER_MT_REGKEY);
  if (listener->fd < 0) luaL_error(L, "Attempt to close a closed listener");
  addrlen = sizeof(addr);
  if (getsockname(listener->fd, (struct sockaddr *)&addr, &addrlen)) addrlen = 0;
  if (close(listener->fd)) {
    moonbr_io_prepare_errmsg();
    listener->fd = -1;
    if (addrlen && addrlen <= sizeof(addr)) {
      if (stat(addr.sun_path, &sb) == 0) {
        if (S_ISSOCK(sb.st_mode)) unlink(addr.sun_path);
      }
    }
    moonbr_io_return_prepared_errmsg();
  }
  listener->fd = -1;
  if (addrlen && addrlen <= sizeof(addr)) {
    if (stat(addr.sun_path, &sb) == 0) {
      if (S_ISSOCK(sb.st_mode)) unlink(addr.sun_path);
    }
  }
  lua_pushboolean(L, 1);
  return 1;
}

static int moonbr_io_listenergc(lua_State *L) {
  moonbr_io_listener_t *listener;
  listener = luaL_checkudata(L, 1, MOONBR_IO_LISTENER_MT_REGKEY);
  if (listener->fd >= 0) close(listener->fd);
  listener->fd = -1;
  return 0;
}

static int moonbr_io_exec(lua_State *L) {
  char **argv;
  int i, argc;
  int sockin[2], sockout[2], sockerr[2];
  volatile int errorcond = 0;
  volatile char errmsgbuf[MOONBR_IO_MAXSTRERRORLEN] = MOONBR_IO_STRERROR_R_MSG;
  moonbr_io_child_t *child;
  argc = lua_gettop(L);
  argv = lua_newuserdata(L, (argc + 1) * sizeof(char *));
  for (i=0; i<argc; i++) argv[i] = (char *)luaL_checkstring(L, i+1);
  argv[argc] = NULL;
  child = lua_newuserdata(L, sizeof(moonbr_io_child_t));
  child->pid = 0;
  lua_newtable(L);
  lua_setuservalue(L, -2);
  luaL_setmetatable(L, MOONBR_IO_CHILD_MT_REGKEY);
  if (socketpair(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, sockin)) {
    moonbr_io_prepare_errmsg();
    lua_pushnil(L);
    lua_pushfstring(L, "Could not create socket pair: %s", errmsg);
    return 2;
  }
  if (socketpair(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, sockout)) {
    moonbr_io_prepare_errmsg();
    close(sockin[0]);
    close(sockin[1]);
    lua_pushnil(L);
    lua_pushfstring(L, "Could not create socket pair: %s", errmsg);
    return 2;
  }
  if (socketpair(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, sockerr)) {
    moonbr_io_prepare_errmsg();
    close(sockin[0]);
    close(sockin[1]);
    close(sockout[0]);
    close(sockout[1]);
    lua_pushnil(L);
    lua_pushfstring(L, "Could not create socket pair: %s", errmsg);
    return 2;
  }
  child->pid = vfork();
  if (child->pid == -1) {
    moonbr_io_prepare_errmsg();
    close(sockin[0]);
    close(sockin[1]);
    close(sockout[0]);
    close(sockout[1]);
    close(sockerr[0]);
    close(sockerr[1]);
    lua_pushnil(L);
    lua_pushfstring(L, "Could not fork: %s", errmsg);
    return 2;
  }
  if (!child->pid) {
    if (dup2(sockin[1], 0) == -1) goto moonbr_io_exec_error1;
    if (dup2(sockout[1], 1) == -1) goto moonbr_io_exec_error1;
    if (dup2(sockerr[1], 2) == -1) goto moonbr_io_exec_error1;
    closefrom(3);
    if (fcntl(0, F_SETFD, 0) == -1) goto moonbr_io_exec_error1;
    if (fcntl(1, F_SETFD, 0) == -1) goto moonbr_io_exec_error1;
    if (fcntl(2, F_SETFD, 0) == -1) goto moonbr_io_exec_error1;
    if (execvp(argv[0], argv)) {
      errorcond = 2;
      strerror_r(errno, (char *)errmsgbuf, MOONBR_IO_MAXSTRERRORLEN);
      _exit(0);
    }
    moonbr_io_exec_error1:
    errorcond = 1;
    strerror_r(errno, (char *)errmsgbuf, MOONBR_IO_MAXSTRERRORLEN);
    _exit(0);
  }
  close(sockin[1]);
  close(sockout[1]);
  close(sockerr[1]);
  if (errorcond) {
    int status;
    close(sockin[0]);
    close(sockout[0]);
    close(sockerr[0]);
    while (waitpid(child->pid, &status, 0) == -1) {
      if (errno != EINTR) {
        moonbr_io_prepare_errmsg();
        luaL_error(L, "Error in waitpid call after unsuccessful exec: %s", errmsg);
      }
    }
    child->pid = 0;
    lua_pushnil(L);
    if (errorcond == 2) lua_pushfstring(L, "Could not execute: %s", errmsgbuf);
    else lua_pushfstring(L, "Error in fork: %s", errmsgbuf);
    return 2;
  }
  lua_pushcfunction(L, moonbr_io_pushhandle_impl);
  lua_pushlightuserdata(L, &sockin[0]);
  if (lua_pcall(L, 1, 1, 0)) {
    if (sockin[0] != -1) close(sockin[0]);
    close(sockout[0]);
    close(sockerr[0]);
    goto moonbr_io_exec_error2;
  }
  lua_setfield(L, -2, "stdin");
  lua_pushcfunction(L, moonbr_io_pushhandle_impl);
  lua_pushlightuserdata(L, &sockout[0]);
  if (lua_pcall(L, 1, 1, 0)) {
    if (sockout[0] != -1) close(sockout[0]);
    close(sockerr[0]);
    goto moonbr_io_exec_error2;
  }
  lua_setfield(L, -2, "stdout");
  lua_pushcfunction(L, moonbr_io_pushhandle_impl);
  lua_pushlightuserdata(L, &sockerr[0]);
  if (lua_pcall(L, 1, 1, 0)) {
    if (sockerr[0] != -1) close(sockerr[0]);
    goto moonbr_io_exec_error2;
  }
  lua_setfield(L, -2, "stderr");
  return 1;
  moonbr_io_exec_error2:
  {
    int status;
    while (waitpid(child->pid, &status, 0) == -1) {
      if (errno != EINTR) {
        moonbr_io_prepare_errmsg();
        luaL_error(L, "Error in waitpid call after error creating socket handles: %s", errmsg);
      }
    }
  }
  child->pid = 0;
  return lua_error(L);
}

static int moonbr_io_childindex(lua_State *L) {
  luaL_checkudata(L, 1, MOONBR_IO_CHILD_MT_REGKEY);
  luaL_checkany(L, 2);
  lua_getuservalue(L, 1);
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  if (lua_isnil(L, -1)) {
    luaL_getmetatable(L, MOONBR_IO_CHILD_PT_REGKEY);
    lua_pushvalue(L, 2);
    lua_gettable(L, -2);
  }
  return 1;
}

static int moonbr_io_childnewindex(lua_State *L) {
  luaL_checkudata(L, 1, MOONBR_IO_CHILD_MT_REGKEY);
  luaL_checkany(L, 2);
  luaL_checkany(L, 3);
  lua_getuservalue(L, 1);
  lua_pushvalue(L, 2);
  lua_pushvalue(L, 3);
  lua_settable(L, -3);
  return 0;
}

static int moonbr_io_childgc(lua_State *L) {
  moonbr_io_child_t *child;
  child = luaL_checkudata(L, 1, MOONBR_IO_CHILD_MT_REGKEY);
  if (child->pid) {
    int status;
    int pid = child->pid;
    child->pid = 0;
    if (kill(pid, SIGKILL)) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Error in kill call during garbage collection: %s", errmsg);
    }
    while (waitpid(pid, &status, 0) == -1) {
      if (errno != EINTR) {
        moonbr_io_prepare_errmsg();
        luaL_error(L, "Error in waitpid call during garbage collection: %s", errmsg);
      }
    }
  }
  return 0;
}

static int moonbr_io_kill(lua_State *L) {
  moonbr_io_child_t *child;
  int sig;
  child = luaL_checkudata(L, 1, MOONBR_IO_CHILD_MT_REGKEY);
  sig = luaL_optinteger(L, 2, SIGKILL);
  if (!child->pid) luaL_error(L, "Attempt to kill an already collected child process");
  if (kill(child->pid, sig)) {
    moonbr_io_prepare_errmsg();
    luaL_error(L, "Error in kill call: %s", errmsg);
  }
  lua_settop(L, 1);
  return 1;
}

static int moonbr_io_wait_impl(lua_State *L, int nonblocking) {
  moonbr_io_child_t *child;
  pid_t waitedpid;
  int status;
  child = luaL_checkudata(L, 1, MOONBR_IO_CHILD_MT_REGKEY);
  if (!child->pid) luaL_error(L, "Attempt to wait for an already collected child process");
  while ((waitedpid = waitpid(child->pid, &status, nonblocking ? WNOHANG : 0)) == -1) {
    if (errno != EINTR) {
      moonbr_io_prepare_errmsg();
      luaL_error(L, "Error in waitpid call: %s", errmsg);
    }
  }
  if (!waitedpid) {
    lua_pushboolean(L, 0);
    lua_pushliteral(L, "Process is still running");
    return 2;
  } else {
    child->pid = 0;
    if (WIFEXITED(status)) {
      lua_pushinteger(L, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      lua_pushinteger(L, -WTERMSIG(status));
    } else {
      luaL_error(L, "Unexpected status value returned by waitpid call");
    }
    return 1;
  }
}

static int moonbr_io_wait(lua_State *L) {
  return moonbr_io_wait_impl(L, 0);
}

static int moonbr_io_wait_nb(lua_State *L) {
  return moonbr_io_wait_impl(L, 1);
}

#if LUA_VERSION_NUM >= 503
static int moonbr_io_wait_cont(lua_State *L, int status, lua_KContext ctx) {
#else
static int moonbr_io_wait_cont(lua_State *L) {
#endif
#if !(LUA_VERSION_NUM >= 503)
  int ctx = 0;
  lua_getctx(L, &ctx);
#endif
  while (1) {
    lua_pushcfunction(L, moonbr_io_wait_nb);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 1);
    if (!lua_isnil(L, -1)) break;
    lua_pushvalue(L, 2);
    lua_callk(L, 0, 0, ctx, moonbr_io_wait_cont);
  }
  return 1;
}

static int moonbr_io_wait_call(lua_State *L) {
  lua_settop(L, 2);
#if LUA_VERSION_NUM >= 503
  return moonbr_io_wait_cont(L, 0, 0);
#else
  return moonbr_io_wait_cont(L);
#endif
}

moonbr_io_yield_wrapper(moonbr_io_wait_yield, moonbr_io_wait_call);

static void moonbr_io_sigterm_handler(int sig) {
  moonbr_io_sigterm_flag = 1;
}

static void moonbr_io_sigchld_handler(int sig) {
  moonbr_io_sigchld_flag = 1;
}

int moonbr_io_catch_sigterm(lua_State *L) {
  signal(SIGTERM, moonbr_io_sigterm_handler);
  return 0;
}

static int moonbr_io_getpid(lua_State *L) {
  lua_pushinteger(L, getpid());
  return 1;
}

#ifdef MOONBR_IO_USE_TLS

#define moonbr_io_poll_tls() \
  if (!handle->tlshandshake) { \
    force_wakeup = 1; \
    continue; \
  } \
  if (handle->tlshandshake == TLS_WANT_POLLIN) { \
    if (fd < 0) { \
      force_wakeup = 1; \
      continue; \
    } \
    FD_SET(fd, &readfds); \
    if (fd+1 > nfds) nfds = fd+1; \
    continue; \
  } \
  if (handle->tlshandshake == TLS_WANT_POLLOUT) { \
    if (fd < 0) { \
      force_wakeup = 1; \
      continue; \
    } \
    FD_SET(fd, &writefds); \
    if (fd+1 > nfds) nfds = fd+1; \
    continue; \
  } \
  while (0)

#endif /* MOONBR_IO_USE_TLS */

static int moonbr_io_poll(lua_State *L) {
  moonbr_io_handle_t *handle;
  moonbr_io_listener_t *listener;
  moonbr_io_child_t *child;
  int fd, isnum;
  int nfds = 0;
  fd_set readfds, writefds, exceptfds;
  struct timespec timeout = {0, };
  int force_wakeup = 0;
  int use_timeout = 0;  // negative for negative timeout
  int check_sigterm = 0;
  int check_sigchld = 0;
  pid_t waitedpid;
  sigset_t mask, orig_mask;
  int status;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  if (!lua_isnoneornil(L, 1)) {
    luaL_checktype(L, 1, LUA_TTABLE);
    for (lua_pushnil(L); lua_next(L, 1); lua_pop(L, 1)) {
      if (lua_toboolean(L, -1)) {
        handle = luaL_testudata(L, -2, MOONBR_IO_HANDLE_MT_REGKEY);
        if (handle) {
          if (handle->closed) luaL_error(L, "Attempt to poll a closed connection");
          fd = handle->fd;
#if MOONBR_IO_USE_TLS
          moonbr_io_poll_tls();
#endif
          if (
            fd < 0 ||  /* fake EOF to simulate shutdown if fd < 0 */
            handle->readbufin != handle->readbufout  /* data pending in buffer */
          ) {
            force_wakeup = 1;
            continue;
          }
        } else {
          listener = luaL_testudata(L, -2, MOONBR_IO_LISTENER_MT_REGKEY);
          if (listener) {
            fd = listener->fd;
            if (fd < 0) luaL_error(L, "Attempt to poll a closed listener");
          } else {
            child = luaL_testudata(L, -2, MOONBR_IO_CHILD_MT_REGKEY);
            if (child) {
              if (!child->pid) luaL_error(L, "Attemt to poll an already collected child process");
              if (!check_sigchld) {
                check_sigchld = 1;
                moonbr_io_sigchld_flag = 0;
                signal(SIGCHLD, moonbr_io_sigchld_handler);
              }
              while ((waitedpid = waitpid(child->pid, &status, WNOHANG|WNOWAIT)) == -1) {
                if (errno != EINTR) {
                  moonbr_io_prepare_errmsg();
                  luaL_error(L, "Error in waitpid call: %s", errmsg);
                }
              }
              if (waitedpid) force_wakeup = 1;
              continue;
            } else {
              fd = lua_tointegerx(L, -2, &isnum);
              if (!isnum) luaL_error(L, "Expected integer (file descriptor), I/O handle, or listener in table key");
            }
          }
        }
        if (fd < 0 || fd >= FD_SETSIZE) luaL_error(L, "File descriptor out of valid range");
        FD_SET(fd, &readfds);
        if (fd+1 > nfds) nfds = fd+1;
      }
    }
  }
  if (!lua_isnoneornil(L, 2)) {
    luaL_checktype(L, 2, LUA_TTABLE);
    for (lua_pushnil(L); lua_next(L, 2); lua_pop(L, 1)) {
      if (lua_toboolean(L, -1)) {
        handle = luaL_testudata(L, -2, MOONBR_IO_HANDLE_MT_REGKEY);
        if (handle) {
          if (handle->closed) luaL_error(L, "Attempt to poll a closed connection");
          if (handle->finished) luaL_error(L, "Attempt to write-poll a finished connection");
          fd = handle->fd;
#if MOONBR_IO_USE_TLS
          moonbr_io_poll_tls();
#endif
        } else {
          listener = luaL_testudata(L, -2, MOONBR_IO_LISTENER_MT_REGKEY);
          if (listener) luaL_error(L, "Attempt to write-poll a listener");
          fd = lua_tointegerx(L, -2, &isnum);
          if (!isnum) luaL_error(L, "Expected integer (file descriptor) or I/O handle in table key");
        }
        if (fd < 0 || fd >= FD_SETSIZE) luaL_error(L, "File descriptor out of valid range");
        FD_SET(fd, &writefds);
        if (fd+1 > nfds) nfds = fd+1;
      }
    }
  }
  if (!lua_isnoneornil(L, 3)) {
    lua_Number n;
    n = lua_tonumberx(L, 3, &isnum);
    if (isnum && n<0) {
      use_timeout = -1;
    } else if (isnum && n>=0 && n<100000000) {
      use_timeout = 1;
      timeout.tv_sec = n;
      timeout.tv_nsec = 1e9 * (n - timeout.tv_sec);
    } else {
      luaL_argcheck(L, 0, 3, "not a valid timeout");
    }
  }
  if (use_timeout < 0) force_wakeup = 1;
  if (!lua_isnoneornil(L, 4)) luaL_checktype(L, 4, LUA_TBOOLEAN);
  check_sigterm = lua_toboolean(L, 4);
  if ((check_sigterm || check_sigchld) && !force_wakeup) {
    sigemptyset(&mask);
    if (check_sigterm) sigaddset(&mask, SIGTERM);
    if (check_sigchld) sigaddset(&mask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask)) abort();
  }
  if (check_sigterm && moonbr_io_sigterm_flag) {
    if (!force_wakeup) {
      if (sigprocmask(SIG_SETMASK, &orig_mask, NULL)) abort();
    }
    lua_pushboolean(L, 0);
    lua_pushliteral(L, "SIGTERM received");
    lua_pushboolean(L, 1);
    return 3;
  }
  if (check_sigchld && !force_wakeup && moonbr_io_sigchld_flag) {
    if (sigprocmask(SIG_SETMASK, &orig_mask, NULL)) abort();
    force_wakeup = 1;
  }
  if (use_timeout < 0) {
    lua_pushboolean(L, 0);
    lua_pushliteral(L, "Timeout");
    if (check_sigterm) {
      lua_pushboolean(L, 0);
      return 3;
    } else {
      return 2;
    }
  }
  if (!force_wakeup) {
    status = pselect(
      nfds, &readfds, &writefds, &exceptfds,
      use_timeout ? &timeout : NULL,
      (check_sigterm || check_sigchld) ? &orig_mask : NULL
    );
    if (check_sigterm || check_sigchld) {
      if (sigprocmask(SIG_SETMASK, &orig_mask, NULL)) abort();
      if (check_sigterm && moonbr_io_sigterm_flag) {
        lua_pushboolean(L, 0);
        lua_pushliteral(L, "SIGTERM received");
        lua_pushboolean(L, 1);
        return 3;
      }
    }
    if (status == -1) {
      if (errno == EINTR) {
        lua_pushboolean(L, 1);
        return 1;
      } else {
        moonbr_io_prepare_errmsg();
        return luaL_error(L, "Unexpected error during \"select\" system call: %s", errmsg);
      }
    } else if (status == 0) {
      lua_pushboolean(L, 0);
      lua_pushliteral(L, "Timeout");
      if (check_sigterm) {
        lua_pushboolean(L, 0);
        return 3;
      } else {
        return 2;
      }
    }
  }
  lua_pushboolean(L, 1);
  return 1;
}

static int moonbr_io_timeref(lua_State *L) {
  lua_Number sub;
  struct timespec tp;
  sub = luaL_optnumber(L, 1, 0);
  if (clock_gettime(CLOCK_MONOTONIC, &tp)) {
    return luaL_error(L, "Could not access CLOCK_MONOTONIC");
  }
  lua_pushnumber(L, tp.tv_sec + tp.tv_nsec / 1.0e9 - sub);
  return 1;
}

#ifdef MOONBR_IO_USE_TLS

#define moonbr_io_tlsconf_string(name, field, func) \
  /* NOTE: use valuetype = lua_getfield(...) for LUA_VERSION_NUM >= 503 */ \
  lua_getfield(L, 1, (field)); \
  valuetype = lua_type(L, -1); \
  if (valuetype != LUA_TNIL) { \
    luaL_argcheck(L, valuetype == LUA_TSTRING, 1, "field \"" field "\" is not a string"); \
    value = lua_tostring(L, -1); \
    if (func(tlsconf->config, value)) { \
      lua_pushnil(L); \
      lua_pushfstring(L, "Could not set " name " \"%s\"", value); \
      return 2; \
    } \
  } \
  lua_pop(L, 1);

#define moonbr_io_tlsconf_binary(name, field, func) \
  /* NOTE: use valuetype = lua_getfield(...) for LUA_VERSION_NUM >= 503 */ \
  lua_getfield(L, 1, (field)); \
  valuetype = lua_type(L, -1); \
  if (valuetype != LUA_TNIL) { \
    luaL_argcheck(L, valuetype == LUA_TSTRING, 1, "field \"" field "\" is not a string"); \
    value = lua_tolstring(L, -1, &valuelen); \
    if (func(tlsconf->config, (void *)value, valuelen)) { \
      lua_pushnil(L); \
      lua_pushliteral(L, "Could not set " name); \
      return 2; \
    } \
  } \
  lua_pop(L, 1);

static int moonbr_io_tlsconf(lua_State *L) {
  moonbr_io_tlsconf_t *tlsconf;
  int valuetype;
  const char *value;
  size_t valuelen;
  luaL_checktype(L, 1, LUA_TTABLE);
  tlsconf = lua_newuserdata(L, sizeof(moonbr_io_tlsconf_t));
  tlsconf->config = tls_config_new();
  if (!tlsconf->config) {
    return luaL_error(L, "Could not allocate memory for TLS configuration");
  }
  luaL_setmetatable(L, MOONBR_IO_TLSCONF_MT_REGKEY);
  lua_getfield(L, 1, "mode");
  value = lua_tostring(L, -1);
  if (value && !strcmp(value, "server")) tlsconf->server = 1;
  else if (value && !strcmp(value, "client")) tlsconf->server = 0;
  else luaL_argcheck(L, 0, 1, "field \"mode\" must be set to \"server\" or \"client\"");
  lua_pop(L, 1);
  moonbr_io_tlsconf_string("CA file",          "ca_file",   tls_config_set_ca_file);
  moonbr_io_tlsconf_string("CA path",          "ca_path",   tls_config_set_ca_path);
  moonbr_io_tlsconf_binary("CA",               "ca_mem",    tls_config_set_ca_mem);
  moonbr_io_tlsconf_string("certificate file", "cert_file", tls_config_set_cert_file);
  moonbr_io_tlsconf_binary("certificate",      "cert_mem",  tls_config_set_cert_mem);
  moonbr_io_tlsconf_string("key file",         "key_file",  tls_config_set_key_file);
  moonbr_io_tlsconf_binary("key",              "key_mem",   tls_config_set_key_mem);
#if LUA_VERSION_NUM >= 503
  valuetype = lua_getfield(L, 1, "verify_client");
#else
  lua_getfield(L, 1, "verify_client");
#endif
  if (lua_toboolean(L, -1)) {
    value = lua_tostring(L, -1);
    if (value && !strcmp(value, "required")) {
      tls_config_verify_client(tlsconf->config);
    } else if (value && !strcmp(value, "optional")) {
      tls_config_verify_client_optional(tlsconf->config);
    } else {
      luaL_argcheck(L, 0, 1, "field \"verify_client\" must be set to \"required\", \"optional\", or be false or nil");
    }
  }
  lua_pop(L, 1);
  // TODO: configurable legacy support
  // tls_config_set_protocols(tlsconf->config, TLS_PROTOCOLS_ALL);
  // tls_config_set_ciphers(tlsconf->config, "legacy");
  return 1;
}

static int moonbr_io_tlsconfgc(lua_State *L) {
  moonbr_io_tlsconf_t *tlsconf;
  tlsconf = luaL_checkudata(L, 1, MOONBR_IO_TLSCONF_MT_REGKEY);
  if (tlsconf->config) tls_config_free(tlsconf->config);
  tlsconf->config = NULL;
  return 0;
}

static int moonbr_io_starttls(lua_State *L) {
  moonbr_io_handle_t *handle;
  moonbr_io_tlsconf_t *tlsconf;
  const char *servername;
  struct tls *tls, *tls2;
  handle = luaL_checkudata(L, 1, MOONBR_IO_HANDLE_MT_REGKEY);
  if (lua_type(L, 2) == LUA_TTABLE) {
    lua_pushcfunction(L, moonbr_io_tlsconf);
    lua_pushvalue(L, 2);
    lua_call(L, 1, 2);
    if (lua_isnil(L, -2)) return 2;
    lua_pop(L, 1);
    lua_replace(L, 2);
  }
  tlsconf = luaL_checkudata(L, 2, MOONBR_IO_TLSCONF_MT_REGKEY);
  if (handle->closed) return luaL_error(L, "Attempt to start TLS on a closed I/O handle");
  if (handle->finished) return luaL_error(L, "Attempt to start TLS on a finished I/O handle");
  if (handle->tls) return luaL_error(L, "Attempt to start TLS twice");
  if (handle->readbufin || handle->writebufin) {
    return luaL_error(L, "Attempt to start TLS on an I/O handle with non-empty buffers");
  }
  if (tlsconf->server) tls = tls_server();
  else {
    servername = luaL_checkstring(L, 3);
    tls = tls_client();
  }
  if (!tls) {
    return luaL_error(L, "Could not allocate memory for TLS context");
  }
  if (tls_configure(tls, tlsconf->config)) goto moonbr_io_starttls_error;
  if (tlsconf->server) {
    if (tls_accept_socket(tls, &tls2, handle->fd)) goto moonbr_io_starttls_error;
    handle->servertls = tls;
    handle->tls = tls2;
  } else {
    if (tls_connect_socket(tls, handle->fd, servername)) goto moonbr_io_starttls_error;
    handle->tls = tls;
  }
  lua_settop(L, 1);
  return 1;
  moonbr_io_starttls_error:
  lua_pushnil(L);
  lua_pushstring(L, tls_error(tls));
  tls_free(tls);
  return 2;
}

#endif /* MOONBR_IO_USE_TLS */

static const struct luaL_Reg moonbr_io_handle_methods[] = {
  {"read", moonbr_io_read},
  {"read_nb", moonbr_io_read_nb},
  {"read_call", moonbr_io_read_call},
  {"read_yield", moonbr_io_read_yield},
  {"drain", moonbr_io_drain},
  {"drain_nb", moonbr_io_drain_nb},
  {"drain_call", moonbr_io_drain_call},
  {"drain_yield", moonbr_io_drain_yield},
  {"write", moonbr_io_write},
  {"write_nb", moonbr_io_write_nb},
  {"write_call", moonbr_io_write_call},
  {"write_yield", moonbr_io_write_yield},
  {"flush", moonbr_io_flush},
  {"flush_nb", moonbr_io_flush_nb},
  {"flush_call", moonbr_io_flush_call},
  {"flush_yield", moonbr_io_flush_yield},
  {"finish", moonbr_io_finish},
  {"close", moonbr_io_close},
  {"reset", moonbr_io_reset},
#ifdef MOONBR_IO_USE_TLS
  {"starttls", moonbr_io_starttls},
#endif
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_handle_metamethods[] = {
  {"__index", moonbr_io_handleindex},
  {"__newindex", moonbr_io_handlenewindex},
  {"__gc", moonbr_io_handlegc},
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_listener_methods[] = {
  {"accept", moonbr_io_accept},
  {"accept_nb", moonbr_io_accept_nb},
  {"close", moonbr_io_unlisten},
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_listener_metamethods[] = {
  {"__gc", moonbr_io_listenergc},
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_child_methods[] = {
  {"kill", moonbr_io_kill},
  {"wait", moonbr_io_wait},
  {"wait_nb", moonbr_io_wait_nb},
  {"wait_call", moonbr_io_wait_call},
  {"wait_yield", moonbr_io_wait_yield},
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_child_metamethods[] = {
  {"__index", moonbr_io_childindex},
  {"__newindex", moonbr_io_childnewindex},
  {"__gc", moonbr_io_childgc},
  {NULL, NULL}
};

static const struct luaL_Reg moonbr_io_module_funcs[] = {
  {"localconnect", moonbr_io_localconnect},
  {"localconnect_nb", moonbr_io_localconnect_nb},
  {"tcpconnect", moonbr_io_tcpconnect},
  {"tcpconnect_nb", moonbr_io_tcpconnect_nb},
  {"locallisten", moonbr_io_locallisten},
  {"tcplisten", moonbr_io_tcplisten},
  {"exec", moonbr_io_exec},
  {"catch_sigterm", moonbr_io_catch_sigterm},
  {"getpid", moonbr_io_getpid},
  {"poll", moonbr_io_poll},
  {"timeref", moonbr_io_timeref},
#ifdef MOONBR_IO_USE_TLS
  {"tlsconf", moonbr_io_tlsconf},
#endif
  {NULL, NULL}
};

#ifdef MOONBR_IO_USE_TLS

static const struct luaL_Reg moonbr_io_tlsconf_metamethods[] = {
  {"__gc", moonbr_io_tlsconfgc},
  {NULL, NULL}
};

#endif /* MOONBR_IO_USE_TLS */

int luaopen_moonbridge_io(lua_State *L) {

  signal(SIGPIPE, SIG_IGN);  /* generate I/O errors instead of signal 13 */

  lua_newtable(L);  // module

  lua_newtable(L);  // public metatable
  lua_newtable(L);  // handle methods
  luaL_setfuncs(L, moonbr_io_handle_methods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -4, "handle_pt");
  lua_setfield(L, -2, "__index");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_HANDLE_PUBLIC_MT_REGKEY);

  lua_newtable(L);  // handle metatable
  luaL_setfuncs(L, moonbr_io_handle_metamethods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, "handle_mt");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_HANDLE_MT_REGKEY);

  lua_newtable(L);  // listener metatable
  luaL_setfuncs(L, moonbr_io_listener_metamethods, 0);
  lua_newtable(L);  // listener methods
  luaL_setfuncs(L, moonbr_io_listener_methods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -4, "listener_pt");
  lua_setfield(L, -2, "__index");
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, "listener_mt");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_LISTENER_MT_REGKEY);

  lua_newtable(L);  // child methods
  luaL_setfuncs(L, moonbr_io_child_methods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, "child_pt");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_CHILD_PT_REGKEY);
  lua_newtable(L);  // child metatable
  luaL_setfuncs(L, moonbr_io_child_metamethods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, "child_mt");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_CHILD_MT_REGKEY);

#ifdef MOONBR_IO_USE_TLS
  if(tls_init()) {
    return luaL_error(L, "Could not initialize TLS library");
  }
  lua_newtable(L);  // tlsconf metatable
  luaL_setfuncs(L, moonbr_io_tlsconf_metamethods, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -3, "tlsconf_mt");
  lua_setfield(L, LUA_REGISTRYINDEX, MOONBR_IO_TLSCONF_MT_REGKEY);
#endif

  moonbr_io_pushhandle(L, 0);
  lua_setfield(L, -2, "stdin");
  moonbr_io_pushhandle(L, 1);
  lua_setfield(L, -2, "stdout");
  moonbr_io_pushhandle(L, 2);
  lua_setfield(L, -2, "stderr");

  luaL_setfuncs(L, moonbr_io_module_funcs, 0);
  return 1;

}

