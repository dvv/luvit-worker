/*
 * N.B. For testing purpose only
 */

#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"

int lsleep(lua_State* L) {

  lua_pushinteger(L, sleep(luaL_checkinteger(L, 1)));

  return 1;
}

/* ------------------------------------------------------ */

static const luaL_reg exports[] = {
  { "sleep", lsleep },
  { NULL, NULL }
};

LUALIB_API int luaopen_sleep(lua_State *L) {

  lua_newtable(L);
  luaL_register(L, NULL, exports);

  return 1;
}
