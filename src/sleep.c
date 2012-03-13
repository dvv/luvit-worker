/*
 *  Copyright 2012 The Luvit Authors. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

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
