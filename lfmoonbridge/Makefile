# BSD Makefile
# On GNU systems, use bmake.

PLATFORM != uname

.if $(PLATFORM) == "FreeBSD"
# Default configuration for FreeBSD
LUA_INCLUDE ?= /usr/local/include/lua53
LUA_LIBDIR  ?= /usr/local/lib
LUA_LIBRARY ?= lua-5.3
UTIL_FLAGS  ?= -lutil

.elif $(PLATFORM) == "Linux"
# Distinguish between different Linux distributions
DISTRIBUTION != lsb_release -i -s
.if $(DISTRIBUTION) == "Debian"
# Default configuration for Debian
LUA_INCLUDE ?= /usr/include/lua5.3
LUA_LIBDIR  ?= /usr/lib
LUA_LIBRARY ?= lua5.3
UTIL_FLAGS  ?= -ldl -lbsd -lrt
.elif $(DISTRIBUTION) == "Ubuntu"
# Default configuration for Ubuntu
LUA_INCLUDE ?= /usr/include/lua5.3
LUA_LIBDIR  ?= /usr/lib/x86_64-linux-gnu
LUA_LIBRARY ?= lua5.3
UTIL_FLAGS  ?= -ldl -lbsd
.else
# Default configuration for other Linux distributions
LUA_INCLUDE ?= /usr/include
LUA_LIBDIR  ?= /usr/lib
LUA_LIBRARY ?= lua
UTIL_FLAGS  ?= -ldl -lbsd
.endif

.else
# Default configuration for other systems
LUA_INCLUDE ?= /usr/include
LUA_LIBDIR  ?= /usr/lib
LUA_LIBRARY ?= lua
UTIL_FLAGS  ?= -lutil

.endif

.ifdef MOONBR_LUA_PATH
MOONBR_LUA_PATH_DEFINE = "-DMOONBR_LUA_PATH=\"$(MOONBR_LUA_PATH)\""
.endif

.ifdef MOONBR_LUA_CPATH
MOONBR_LUA_CPATH_DEFINE = "-DMOONBR_LUA_CPATH=\"$(MOONBR_LUA_CPATH)\""
.endif

all:: moonbridge moonbridge_io.so

moonbridge: moonbridge.c moonbridge_io.h moonbridge_io.o
	cc -Wall -Wno-unused-result -O2 -Wl,-E -I $(LUA_INCLUDE) -L $(LUA_LIBDIR) -o moonbridge $(MOONBR_LUA_PATH_DEFINE) $(MOONBR_LUA_CPATH_DEFINE) moonbridge.c -lm -l$(LUA_LIBRARY) $(UTIL_FLAGS) moonbridge_io.o

moonbridge_io.o: moonbridge_io.c moonbridge_io.h
	cc -c -Wall -O2 -fPIC -I $(LUA_INCLUDE) -o moonbridge_io.o moonbridge_io.c

moonbridge_io.so: moonbridge_io.o
	ld -shared -o moonbridge_io.so moonbridge_io.o $(UTIL_FLAGS)

freebsd_with_tls::
	#TODO
	cc -D MOONBR_IO_USE_TLS -c -Wall -O2 -fPIC -I /usr/local/include/lua53 -I /usr/local/include -o moonbridge_io.o moonbridge_io.c && ld -L/usr/local/lib -shared -o moonbridge_io.so moonbridge_io.o -lutil -ltls

clean::
	rm -f moonbridge moonbridge_io.o moonbridge_io.so

